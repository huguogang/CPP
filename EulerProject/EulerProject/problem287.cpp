//TODO: all quad scan are natually parallel operations, should be able to get some 4X speed up here
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
const int N = 24;
//use look up instead of pow in recursive search code cuts ~ 40% of the run time
int POWER2[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28};
/* scan based on the test patter to test the encoding algorithm
int image[4][4] = {
{ 0, 0, 0, 1 },
{ 0, 0, 1, 0 },
{ 1, 1, 0, 0},
{ 1, 1, 0, 0}
};
//scan the image, return 0 if all uniform
int scanImage(int x0, int y0, int x1, int y1) {
int p = image[x0][y0];
for (int i = x0; i < x1; i++) {
for (int j = y0; j < y1; j++) {
if (p^image[i][j]) {
return 1; //non-uniform
}
}
}
return 0;
}
*/

int CENTER = POWER2[N - 1];
long long R = long long(pow(2, 2 * N - 2));

//scan a section of the iamge, return 0 if all uniform
// (x-2^(N-1))^2 + (y-2^(N-1))^2 <= 2^(2N-2)
//Note: range of scan [x0, x1), [y0, y1)
int scanImage(int x0, int y0, int x1, int y1) {
    long long x0x0 = (x0 - CENTER);
    x0x0 = x0x0 * x0x0;

    long long y0y0 = (y0 - CENTER);
    y0y0 = y0y0 * y0y0;

    long long x1x1 = (x1 - 1 - CENTER);
    x1x1 = x1x1 * x1x1;

    long long y1y1 = (y1 - 1 - CENTER);
    y1y1 = y1y1 * y1y1;

    bool b = (x0x0 + y0y0 <= R);
    bool b1 = (x0x0 + y1y1 <= R);
    if (b ^ b1) {
        return 1;
    }

    b1 = (x1x1 + y1y1 <= R);
    if (b ^ b1) {
        return 1;
    }

    b1 = (x1x1 + y0y0 <= R);
    if (b ^ b1) {
        return 1;
    }
    //all four corners are black, because it's convex, the whole section is on the same color
    if (b) {
        return 0; //all black
    }
    //all 4 are white, due to the special characteristic of given shape, we just need to check 
    //all 4 edges are white
    for (int i = x0 + 1; i < x1; i++) {
        long long xx = (i - CENTER);
        xx = xx * xx;
        if (xx + y0y0 <= R) {
            return 1;
        }
        if (xx + y1y1 <= R) {
            return 1;
        }

    }

    for (int i = y0 + 1; i < y1; i++) {
        long long yy = i - CENTER;
        yy = yy * yy;
        if (x0x0 + yy <= R) {
            return 1;
        }
        if (x1x1 + yy <= R) {
            return 1;
        }
    }

    return 0; //all white
}

//get code length for the quadrant starting on (x0, y0), with size 2 ^ n
int codeLenR(int x0, int y0, int n) {
    if (n == 0) {
        //one pixel, 2 bit code
        return 2;
    }
    int imageSize = POWER2[n];
    if (scanImage(x0, y0, x0 + imageSize, y0 + imageSize) == 0) {
        //uniform quad, 2 bit code
        return 2;
    }
    int halfSize = imageSize / 2;
    int codeLen = 1 +  //first 0
        codeLenR(x0, y0, n - 1) + //lower left quad
        codeLenR(x0, y0 + halfSize, n - 1) + //uper left quad
        codeLenR(x0 + halfSize, y0, n - 1) +   //lower right quad
        codeLenR(x0 + halfSize, y0 + halfSize, n - 1); //upper right quad
    return codeLen;
}

int solve() {
    //we know first level is not uniform, and this first cut will ensure the assumption
    //about white block is correct;
    int halfSize = int(POWER2[N-1]);
    return 1 + codeLenR(0, 0, N - 1) +
        codeLenR(halfSize, 0, N - 1) +
        codeLenR(0, halfSize, N - 1) +
        codeLenR(halfSize, halfSize, N - 1);
}

int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    int ret = solve();
    cout << ret << endl;
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1) * 1.0 / (CLOCKS_PER_SEC) << endl;
    cin.ignore(1);
    return 0;
}
