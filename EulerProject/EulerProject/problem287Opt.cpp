//solution for problem 287, with optimization that may make it completely unrecognizable
//performance tunes:
//  * look up for power 2, and all the other numeric details (~40%, from 16 to 10 sec)
//  * parallel_for (~50% reduction from 2 sec to 1 sec)
//  * pre-split top level quad
//      - now we just need to check 4 corners for uniformity
//      - cut one quad calculation out due to symmetry
//   * change Visual Studio target from Win32 to x64 (~70% reduction from 1 sec to 0.3 sec), 
//      should due to long long multiplication improvement in native 64bit instructions
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

#include <ppl.h>

using namespace std;
using namespace concurrency;

const int N = 24;
const int POWER2[] = {  1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7,
                        1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15,
                        1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23,
                        1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28 };
const long long ONE = 1;
const int CENTER = POWER2[N - 1];
const long long RR = ONE << (2 * N - 2);

//get code length for the quadrant starting on (x0, y0), with size 2 ^ n
inline int codeLenR(const int& x0, const int& y0, const int& n,
    const long long& x0x0, const long long& x1x1,
    const long long& y0y0, const long long& y1y1) {
    if (n == 0) {
        //one pixel, 2 bit code
        return 2;
    }
    int imageSize = POWER2[n];

    bool b = (x0x0 + y0y0 <= RR);
    if ((b == (x0x0 + y1y1 <= RR) &&
        b == (x1x1 + y1y1 <= RR) &&
        b == (x1x1 + y0y0 <= RR))) {
        //all four corners same color, 2 bit code
        return 2;
    }

    int halfSize = imageSize >> 1;
    long long x1x1left, x0x0right, y1y1lower, y0y0upper;
    x0x0right = (x0 + halfSize - CENTER);
    x1x1left = (x0 + halfSize - CENTER - 1);
    x0x0right *= x0x0right;
    x1x1left *= x1x1left;

    y0y0upper = (y0 + halfSize - CENTER);
    y1y1lower = (y0 + halfSize - CENTER - 1);
    y0y0upper *= y0y0upper;
    y1y1lower *= y1y1lower;
    
    return 1 +   //first 0
        codeLenR(x0, y0, n - 1, x0x0, x1x1left, y0y0, y1y1lower) + //lower left quad
        codeLenR(x0, y0 + halfSize, n - 1, x0x0, x1x1left, y0y0upper, y1y1) + //uper left quad
        codeLenR(x0 + halfSize, y0, n - 1, x0x0right, x1x1, y0y0, y1y1lower) +   //lower right quad
        codeLenR(x0 + halfSize, y0 + halfSize, n - 1, x0x0right, x1x1, y0y0upper, y1y1); //upper right quad
}

//struct to drive the parallel for loop
const int DIR[3][2] = { { 0, 0 }, { 0, 1 }, { 1, 1 } };
const long long HALF = ONE << (N - 1);
const long long HALF_HALF = (HALF - 1) * (HALF - 1);
const long long INTERMEDIATE_PRODUCT[3][4] = {
        { HALF * HALF, 1, HALF * HALF, 1 },
        { HALF * HALF, 1, 0, HALF_HALF },
        { 0, HALF_HALF, 0, HALF_HALF } };
int solve() {
    //we know first level is not uniform, 
    //and this first cut will ensure the assumption
    //about white block is correct (when look at the whole square, we will 
    //have four corners white while the region is still ununiform, but
    //once cut to quad, we can be sure the inside will all be white too)
    int halfSize = int(POWER2[N - 1]);
    int arrLen[3];
    parallel_for(0, 3,
        [&](int i) {
        long long t1, t2, t3, t4;
        t1 = t2 = t3 = t4 = -1;
        arrLen[i] = codeLenR(halfSize * DIR[i][0],
            halfSize * DIR[i][1], N - 1,
            INTERMEDIATE_PRODUCT[i][0],
            INTERMEDIATE_PRODUCT[i][1],
            INTERMEDIATE_PRODUCT[i][2],
            INTERMEDIATE_PRODUCT[i][3]);
    });
    return 1 + arrLen[0] +
        arrLen[1] * 2 +     //symmetry, arrLen[1] == arrLen[3]
        arrLen[2];
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
