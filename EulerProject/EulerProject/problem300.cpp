// - mirror symmetry
// - pre-calculate upper bound for a given number of H, search can stop if 
//   reached upper bound
// - lower bound is existing number of bonds when arranged in straight line
// - pre-calculate (and extract info) of all possible layout (folding) of the given length
//   remove rotation, mirror equivalents)
//     * also remove layout that does not generate any new bonds (light straight line)
// - stop and trackback the folding if no block to place the next element
// - actually, forget about rotation, mirror, ... for what we care about, the unique
//   combo of additional contact points defines a unique layout of interest
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <set>
#include <vector>
#include <limits>
#include <string>
using namespace std;

int solveOneString(vector<char>string)
int solve() {
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    string s = "HHPPHHHPHHPH";
    int ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / (CLOCKS_PER_SEC) << endl;
    return 0;
}