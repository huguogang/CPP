//v1 total run time: 12 sec
//v2: add mirror removal, run time: 6 sec
//v2: adopt parallel_for: 1.4 sec (quad-core, hyper-thread, ~ 8 cores, 25% runtime)
//A few other ideas from forum that should provide further improvements
// * save new bond in the fold as bit masks (check new H bond will be faster)
// * remove folds whose new bonds are strictly subset of another fold
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <set>
#include <vector>
#include <limits>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>

//these two are for parallel_for
//#include <ppl.h>
#include <concurrent_vector.h>

using namespace std;
using namespace concurrency;

//length of the string
const int LEN = 15;
//for what we care about, fold is defined by new bonds created by fold
//the bonds should be sorted by first, than seconds to facilitate the set opration
typedef vector < pair<int, int> > FOLD;
struct FOLD_COMPARE {
    bool operator() (const FOLD& lhs, const FOLD& rhs) const{
        if (lhs.size() == rhs.size()) {
            return lhs > rhs;
        }
        return lhs.size() > rhs.size();
    }
};
//all unique combinations of folds of given length, 
//ordered by number of new bonds descending order
set<FOLD, FOLD_COMPARE> allFolds;

//matrix for temporary storage of fold pattern
int matrix[LEN * 2 - 1][LEN * 2 - 1];
//all 4 possible directions
int dir[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
//record the growing string
vector<pair<int, int>> q;


void processFold() {
    FOLD f; //give plenty of initial space, otherwise allocator will cost a lot
    //scan for new bonds
    for (int i = 0; i < LEN; i++) {
        auto p1 = q[i];
        for (int j = 0; j < 4; j++) {
            int row = p1.first + dir[j][0];
            int col = p1.second + dir[j][1];
            if (row < 0 || col < 0 ||
                row >= LEN * 2 - 1 || col >= LEN * 2 - 1) {
                //out of bound, no new bond
                continue;
            }
            int val = matrix[row][col];
            if (abs(val - (i + 1)) == 1 ||      //not a new bond
                val == 0 ||                     //no element
                val < i + 1                     //duplicated bond, only keep first < second bond
                ) {
                continue;
            }
            f.push_back(pair<int, int>(i + 1, val));
        }
    }
    //sort by first then second element in the bond
    std::sort(f.begin(), f.end(), [](pair<int, int> a, pair<int, int> b) {
        if (a.first == b.first) {
            return a.second > b.second;
        }
        return a.first > b.first;
    });
    //add to set
    allFolds.insert(f);
}

//add a point during search for a fold
void addPoint(int level, int row, int col) {
    q.push_back(pair<int, int>(row, col));
    matrix[row][col] = level;
}

//pop last search point
void popPoint() {
    auto p = q.back();
    matrix[p.first][p.second] = 0;
    q.pop_back();
}

void genFoldsR(int level, int row, int col) {
    if (level > LEN) {
        //got a legit full length string
        processFold();
        return;
    }

    for (int i = 0; i < 4; i++) {
        int newRow = row + dir[i][0];
        int newCol = col + dir[i][1];
        if (matrix[newRow][newCol] == 0) {
            addPoint(level, newRow, newCol);
            genFoldsR(level + 1, newRow, newCol);
            popPoint();
        }
    }
}

void generateFolds() {
    //first point start from center, so we will never wonder out of bound
    addPoint(1, LEN - 1, LEN - 1);
    //second point always go right, due to symmetry, we will not lose anything
    addPoint(2, LEN - 1, LEN);
    genFoldsR(3, LEN - 1, LEN);
}

int countExistingBond(int protein) {
    int prev = 1;
    int mask = 1;
    int ret = 0;
    for (int i = 0; i < LEN; i++) {
        int bit = protein & mask;
        if (prev == 0 && bit == 0) {
            ret++;
        }
        prev = bit;
        mask = mask << 1;
    }
    return ret;
}

int countNewBonds(int protein, const FOLD& fold) {
    int ret = 0;
    for (auto p = fold.begin(); p != fold.end(); p++) {
        if ((protein & (1 << (p->first - 1))) == 0 &&
            (protein & (1 << (p->second - 1))) == 0) {
            ret++;
        }
    }
    return ret;
}

int solve() {
    generateFolds();
    //bit rep of protein, 0: H, 1: P
    int sum = 0;
    concurrent_vector<int> maxBond;
    parallel_for(0, 1 << LEN,
        [&](int i) {
        int head = LEN - 1, tail = 0;
        int mirrorFactor = 1; //2 if mirror exists, 1 applies for HHHH, or PPPP
        while (head > tail) {
            bool h = i & (1 << head);
            bool t = i & (1 << tail);
            if (h == t) {
                //could be palindrome, then we can not rely on mirror
                tail++;
                head--;
            }
            else {
                //head != tail, a mirror exists
                if (h < t) {
                    //we will calculate this one, and skip the other mirrored string
                    mirrorFactor = 2;
                }
                else {
                    return; //skip
                }
                break;
            }
        }

        int maxNewBond = 0;
        int fixed = countExistingBond(i);
        int newBond;
        for (auto fold = allFolds.begin(); fold != allFolds.end(); fold++) {
            if (maxNewBond >= fold->size()) {
                break; //impossible to beat the current best
            }
            newBond = countNewBonds(i, *fold);
            maxNewBond = max(maxNewBond, newBond);
            if (newBond == fold->size()) {
                //all match, this is the best solution, because we search in order of number of new bonds
                break;
            }

        }
        maxBond.push_back((maxNewBond + fixed) * mirrorFactor);
    });
    sum = accumulate(maxBond.begin(), maxBond.end(), 0);
    /* replaced by parallel_for
    int nSkipped = 0;
    int nPal = 0;

    for (int i = 0; i < (1 << LEN); i++) {
        int head = LEN - 1, tail = 0;
        int mirrorFactor = 1; //2 if mirror exists, 1 applies for HHHH, or PPPP
        bool skip = false;
        while (head > tail) {
            bool h = i & (1 << head);
            bool t = i & (1 << tail);
            if (h == t) {
                //could be palindrome, then we can not rely on mirror
                tail++;
                head--;
            }
            else {
                //head != tail, a mirror exists
                if (h < t) {
                    //we will calculate this one, and skip the other mirrored string
                    mirrorFactor = 2;
                }
                else {
                    skip = true;
                    nSkipped++;
                }
                break;
            }
        }
        if (skip) {
            continue;
        }
        if (mirrorFactor == 1) {
            nPal++;
        }
        int maxNewBond = 0;
        int fixed = countExistingBond(i);
        int newBond;
        for (auto fold = allFolds.begin(); fold != allFolds.end(); fold++) {
            if (maxNewBond >= fold->size()) {
                break; //impossible to beat the current best
            }
            newBond = countNewBonds(i, *fold);
            maxNewBond = max(maxNewBond, newBond);
            if (newBond == fold->size()) {
                //all match, this is the best solution, because we search in order of number of new bonds
                break;
            }

        }

        sum += (maxNewBond + fixed) * mirrorFactor;
    }
    cout << "Skipped: " << nSkipped << endl;
    cout << "Palindrome: " << nPal << endl;
    */
    return sum;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    int ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / (CLOCKS_PER_SEC) << endl;
    return 0;
}
*/