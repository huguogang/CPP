#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;
const int WIDTH = 20;
const int HEIGHT = 20;
//lesson learned: integer overflow. No error is generated, a negative number is generated
// for the overflow multiplication. change from int to long long product solved the problem
vector<vector<int>> data = {
        {  8,  2, 22, 97, 38, 15,  0, 40,  0, 75,  4,  5,  7, 78, 52, 12, 50, 77, 91,  8 },
        { 49, 49, 99, 40, 17, 81, 18, 57, 60, 87, 17, 40, 98, 43, 69, 48,  4, 56, 62,  0 },
        { 81, 49, 31, 73, 55, 79, 14, 29, 93, 71, 40, 67, 53, 88, 30,  3, 49, 13, 36, 65 },
        { 52, 70, 95, 23,  4, 60, 11, 42, 69, 24, 68, 56,  1, 32, 56, 71, 37,  2, 36, 91 },
        { 22, 31, 16, 71, 51, 67, 63, 89, 41, 92, 36, 54, 22, 40, 40, 28, 66, 33, 13, 80 },
        { 24, 47, 32, 60, 99,  3, 45,  2, 44, 75, 33, 53, 78, 36, 84, 20, 35, 17, 12, 50 },
        { 32, 98, 81, 28, 64, 23, 67, 10, 26, 38, 40, 67, 59, 54, 70, 66, 18, 38, 64, 70 },
        { 67, 26, 20, 68,  2, 62, 12, 20, 95, 63, 94, 39, 63,  8, 40, 91, 66, 49, 94, 21 },
        { 24, 55, 58,  5, 66, 73, 99, 26, 97, 17, 78, 78, 96, 83, 14, 88, 34, 89, 63, 72 },
        { 21, 36, 23,  9, 75,  0, 76, 44, 20, 45, 35, 14,  0, 61, 33, 97, 34, 31, 33, 95 },
        { 78, 17, 53, 28, 22, 75, 31, 67, 15, 94,  3, 80,  4, 62, 16, 14,  9, 53, 56, 92 },
        { 16, 39,  5, 42, 96, 35, 31, 47, 55, 58, 88, 24,  0, 17, 54, 24, 36, 29, 85, 57 },
        { 86, 56,  0, 48, 35, 71, 89,  7,  5, 44, 44, 37, 44, 60, 21, 58, 51, 54, 17, 58 },
        { 19, 80, 81, 68,  5, 94, 47, 69, 28, 73, 92, 13, 86, 52, 17, 77,  4, 89, 55, 40 },
        {  4, 52,  8, 83, 97, 35, 99, 16,  7, 97, 57, 32, 16, 26, 26, 79, 33, 27, 98, 66 },
        { 88, 36, 68, 87, 57, 62, 20, 72,  3, 46, 33, 67, 46, 55, 12, 32, 63, 93, 53, 69 },
        {  4, 42, 16, 73, 38, 25, 39, 11, 24, 94, 72, 18,  8, 46, 29, 32, 40, 62, 76, 36 },
        { 20, 69, 36, 41, 72, 30, 23, 88, 34, 62, 99, 69, 82, 67, 59, 85, 74,  4, 36, 16 },
        { 20, 73, 35, 29, 78, 31, 90,  1, 74, 31, 49, 71, 48, 86, 81, 16, 23, 57,  5, 54 },
        {  1, 70, 54, 71, 83, 51, 54, 69, 16, 92, 33, 48, 61, 43, 52,  1, 89, 19, 67, 48 }
};

long long maxProduct(vector<int> v) {
    long long prod = 1;
    int count = 0;
    int len = v.size();
    long long maxProd = 0; //assuming no negative numbers

    for (int i = 0; i < len; i++) {
        if (v[i] == 0) {
            prod = 1;
            count = 0;
            continue;
        }
        prod *= v[i];
        count++;
        if (count > 4) {
            prod /= v[i - 4];
            count--;
        }
        if (count == 4) {
            maxProd = max(prod, maxProd);
            if (maxProd >= 51267216) {
                cout << "here" << endl;
            }
        }
    }
    return maxProd;
}

long long solve() {
    vector<int> tmp;
    long long maxProd = 0;
    //horizontal
    for (int i = 0; i < HEIGHT; i++) {
        long long prod = maxProduct(data[i]);
        maxProd = max(prod, maxProd);
    }

    //vertical
    for (int col = 0; col < WIDTH; col++) {
        tmp.clear();
        for (int row = 0; row < WIDTH; row++) {
            tmp.push_back(data[row][col]);
        }
        long long prod = maxProduct(tmp);
        maxProd = max(prod, maxProd);
    }
    //diagonal
    for (int diag = 0; diag < WIDTH + HEIGHT - 1; diag++) {
        tmp.clear();
        int z = (diag > WIDTH - 1) ? (diag - WIDTH + 1) : 0;
        long long prod;
        //diag with const sum of row and col index
        for (int row = z; row < diag - z + 1; row++) {
            int col = diag - row;
            tmp.push_back(data[row][col]);
        }
        prod = maxProduct(tmp);
        maxProd = max(prod, maxProd);

        //the other direction
        tmp.clear();
        for (int row = z; row < diag - z + 1; row++) {
            int col = row + (WIDTH - 1) - diag;
            tmp.push_back(data[row][col]);
        }
        prod = maxProduct(tmp);
        maxProd = max(prod, maxProd);
    }
    return maxProd;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    long long ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / CLOCKS_PER_SEC << endl;
    return 0;
}
*/