#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

const int N = 100;
const int THRESHOLD = 1000000;

int solve() {
    int count = 0;
    vector<int> previous = { 1, 2, 1 }; //choose from 2
    vector<int> current;

    for (int i = 3; i <= N; i++) {
        current.clear();
        current.push_back(1);
        for (int j = 1; j < i; j++) {
            //n choose k = (n - 1) choose (k - 1) + (n - 1) choose k
            int val = previous[j - 1] + previous[j];
            if (val > THRESHOLD) {
                count++;
                val = THRESHOLD + 1; // avoid overflow
            }
            current.push_back(val);
        }
        current.push_back(1);
        previous.clear();
        previous.swap(current);
    }
    return count;
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
