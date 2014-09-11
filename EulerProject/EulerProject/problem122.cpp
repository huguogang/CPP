//Efficient Exponentiation
//reasoning:
// - For k under 200, m(k) Should not be more than 13, because we can alway using "binary" mathod to build any
//   k under 200 with no more than 13 steps (max 7 steps to build all 2^n, then 6 steps to add them together)
// - In an efficient exponential solution, no exponent shall show up twice
// - Any solution can be executed by sorting result's exponent in strict ascending order
// - m(k) <= Binary exponentiation
//Total 8000 seconds

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <set>
#include <vector>
#include <limits>

using namespace std;
//upper limit for k
const int K_UPPER_LIMIT = 200;
int maxLevel;

vector<int> min(K_UPPER_LIMIT + 1, INT_MAX);
vector<bool> visited(K_UPPER_LIMIT + 1, false);

//depth first
void searchR(int level, vector<int>& path) {
    int head = path[level];
    if (min[head] > level) {
        min[head] = level;
        visited[head] = true;
    }
    
    level++;
    //termination condition
    if (level > maxLevel) {
        return;
    }

    set<int> nextStep;
    for (int i = 0; i < level; i++) {
        for (int j = i; j < level; j++) {
            int next = path[j] + path[i];
            if (next > K_UPPER_LIMIT || next <= head) {
                continue;
            }
            nextStep.insert(next);
        }
    }
    for (auto it = nextStep.begin(); it != nextStep.end(); it++) {
        path.push_back(*it);
        searchR(level, path);
        path.pop_back();
    }
}

int solve() {
    //it helps to set maxLevel as tight as possible
    //for any number, max level is BitLength + Count1s - 2
    maxLevel = 0;
    for (int i = 2; i <= K_UPPER_LIMIT; i++) {
        //levels needed for binary method
        int power = int(floor(log2(i)));
        int count = 0;
        int n = i;
        while (n > 0) {
            n &= n - 1;
            count++;
        }
        if (power + count - 1 > maxLevel) {
            maxLevel = power + count - 1;
            if (maxLevel == 13) {
                cout << i;
            }
        }
    }
    cout << "Max search level: " << maxLevel << endl;
    //init search
    vector<int> path;
    path.push_back(1);
    min[1] = 0;
    min[0] = 0;
    searchR(0, path);

    int sum = 0;
    cout << "-- count for each number --" << endl;
    for (int i = 1; i <= K_UPPER_LIMIT; i++) {
        sum += min[i];
        cout << i << "\t" << min[i] << "\t" << visited[i] << endl;
    }
    cout << "-- end of dump --" << endl;
    return sum;
}

int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    int ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / (CLOCKS_PER_SEC) << endl;
    return 0;
}
