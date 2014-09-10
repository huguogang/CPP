// EulerProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <unordered_set>

using namespace std;
//factorial lookup
int factorial[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

int sumFactDigits(int n) {
    int ret = 0;
    while (n > 0) {
        int lsd = n % 10;
        n = floor(n / 10);
        ret += factorial[lsd];
    }
    return ret;
}
int chainLength(int n) {
    unordered_set<int> set;
    int length = 0;
    while (set.find(n) == set.end()) {
        set.insert(n);
        n = sumFactDigits(n);
        length++;
    }
    return length;
}

int solve() {
    int LENGTH = 60;
    int count = 0;
    for (int i = 1; i < 1000000; i++) {
        int len = chainLength(i);
        if (len == LENGTH) {
            count++;
        }
    }
    return count;
}
int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    int ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1)/CLOCKS_PER_SEC << endl;
    return 0;
}
