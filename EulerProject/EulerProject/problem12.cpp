#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <string>

using namespace std;

int GetFactorCount(long long n) {
    int count = 1;
    for (int i = 2; i <= n; i++) {
        int exp = 0;
        while (n % i == 0) {
            n = n / i;
            exp++;
        }
        count *= (exp + 1);
    }
    return count;
}

long long solve() {
    long long ret = 0;
    int found = false;
    int counter = 0;
    while (!found) {
        counter++;
        ret += counter;
        if (ret < 500) {
            continue;
        }
        if (GetFactorCount(ret) > 500) {
            found = true;
        }
    }
    return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    auto ret = solve();
    //auto ret = GetFactorCount(2*2*2*3*3*107*107);
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / CLOCKS_PER_SEC << endl;
    return 0;
}
