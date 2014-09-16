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

using namespace std;

const int NUM_POINTS = 20;
pair<int, int> points[NUM_POINTS];

long long s = 290797;
int getNextNumber() {
    s = (s * s) % 50515093;
    return (s % 2000) - 1000;
}
void generatePoints() {
    int t;
    for (int i = 0; i < NUM_POINTS; i++) {
        points[i].first = getNextNumber();
        points[i].second = getNextNumber();
    }
}

/*
int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    //int ret = sovle();
    //cout << ret << endl;
    generatePoints();
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / (CLOCKS_PER_SEC) << endl;
    return 0;
}
*/
