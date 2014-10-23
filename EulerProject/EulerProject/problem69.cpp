#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "MathUtils.h"

using namespace std;
//const int UPPER_LIMIT = 1000000;
const int UPPER_LIMIT = 100000;
vector<int> primeNumbers;

int solve() {
    getPrimeNumbers(UPPER_LIMIT, primeNumbers);

    int maxN = 0;
    double maxRatio = 0;

    for (int i = 2; i < UPPER_LIMIT; i++) {
        vector<pair<int, int>> factors;
        factorization(i, primeNumbers, factors);
        double r = (double) i / totient(factors);
        if (r > maxRatio) {
            maxRatio = r;
            maxN = i;
        }
        //cout << i << "\t" << totient(i) << "\t" << r << endl;
    }
    return maxN;
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
