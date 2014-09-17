#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>

using namespace std;

const int N = 1000;
const long long MASK = 10000000000L;

long long solve() {
    long long sum = 0;
    for (int i = 1; i <= N; i++) {
        long long tail = 1;
        for (int j = 0; j < i; j++) {
            tail *= i;
            tail %= MASK;
        }
        sum += tail;
        sum %= MASK;
    }
    return sum;
}

int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    long long ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / (CLOCKS_PER_SEC) << endl;
    return 0;
}
