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
#include <map>
#include <algorithm>

using namespace std;

bool isPrime(int n) {
    if (n == 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}

int getKey(int n) {
    int key;
    vector<int> digits;
    while (n > 0) {
        digits.push_back(n % 10);
        n = floor(n / 10);
    }
    sort(digits.begin(), digits.end());
    key = 0;
    for (auto it = digits.begin(); it != digits.end(); it++) {
        key = key * 10 + *it;
    }
    return key;
}

//give ascending sorted sequnce of numbers, find subset of equal distance
void searchSequence(vector<int> seq) {
    int len = seq.size();
    for (int i = 0; i < len - 2; i++) {
        for (int j = i + 1; j < len - 1; j++) {
            for (int k = j + 1; k < len; k++) {
                if (seq[k] - seq[j] == seq[j] - seq[i]) {
                    cout << seq[i] << ",";
                    cout << seq[j] << ",";
                    cout << seq[k] << endl;
                    return;
                }
            }
        }
    }
}

int solve() {
    map<int, vector<int>> primeNumbers;

    for (int i = 1000; i <= 9999; i++) {
        if (isPrime(i)) { 
            int key = getKey(i);
            if (primeNumbers.find(key) == primeNumbers.end()) {
                vector<int> v;
                primeNumbers[key] = v;
            }
            primeNumbers[key].push_back(i);
        }
    }
    for (auto it = primeNumbers.begin(); it != primeNumbers.end(); it++) {
        if (it->second.size() >= 3) {
            searchSequence(it->second);
        }
    }
    return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    int ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << ((c2 - c1) * 1.0) / (CLOCKS_PER_SEC << endl;
    return 0;
}
