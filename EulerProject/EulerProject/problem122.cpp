//Efficient Exponentiation
//reasoning:
// - For k under 200, m(k) Should not be more than 14, because we can alway using "binary" mathod to build any
//   k under 200 with no more than 14 steps (max 7 steps to build all 2^n, then 7 steps to add them together)
// - Any solution can be executed by sorting result's exponent in ascending order
// - In an efficient exponential solution, no exponent shall show up twice
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <set>
#include <vector>
#include <limits>

using namespace std;

int solve(int max) {
    int sum = 0;
    int nSolved = 0;
    int nStep = 0;
    int solutions[201]; //HACK, we know max 200 for the problem
    vector<vector<int>> paths; //paths at current step
    paths.push_back({ 1 });
    //breath first search
    while (true) {
        nStep++;
        for (auto path = paths.begin(); path != paths.end(); path++) {
            for (auto num = path->begin(); num != path->end(); num++) {
                //int exp = 
            }
        }
    }
    return sum;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    int ret = solve(2);
    cout << ret << endl;
    ret = solve(15);
    cout << ret << endl;
    ret = solve(200);
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) / CLOCKS_PER_SEC << endl;
    return 0;
}
*/
