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

//parallel_for
#include <ppl.h>

using namespace std;
using namespace concurrency;

const int N = 24;
const int POWER2[] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28 };
const int CENTER = POWER2[N - 1];
const long long ONE = 1;
const long long RR = ONE << (2 * N - 2);


//scan a section of the iamge, return 0 if all uniform
// (x-2^(N-1))^2 + (y-2^(N-1))^2 <= 2^(2N-2)
//Note: range of scan [x0, x1), [y0, y1)
int scanImage(const long long& x0x0, const long long& x1x1, const long long& y0y0, const long long& y1y1) {
    bool b = (x0x0 + y0y0 <= RR);
    bool b1 = (x0x0 + y1y1 <= RR);
    if (b ^ b1) {
        return 1;
    }

    b1 = (x1x1 + y1y1 <= RR);
    if (b ^ b1) {
        return 1;
    }

    b1 = (x1x1 + y0y0 <= RR);
    if (b ^ b1) {
        return 1;
    }
    //due to the special location of our ciricle, it is true for both black and white
    return 0;
}
//get code length for the quadrant starting on (x0, y0), with size 2 ^ n
int codeLenR(const int& x0, const int& y0, const int& n, 
            long long x0x0, long long x1x1, long long y0y0, long long y1y1) {
    if (n == 0) {
        //one pixel, 2 bit code
        return 2;
    }
    int imageSize = POWER2[n];
    //performance is now dominantly long long multiply
    if (x0x0 < 0) {
        x0x0 = (x0 - CENTER);
        x0x0 *= x0x0;
    }
    if (y0y0 < 0) {
        y0y0 = (y0 - CENTER);
        y0y0 *= y0y0;
    }

    if (x1x1 < 0) {
        x1x1 = x0 + imageSize - CENTER - 1;
        x1x1 *= x1x1;
    }

    if (y1y1 < 0) {
        y1y1 = y0 + imageSize - CENTER - 1;
        y1y1 *= y1y1;
    }
    if (scanImage(x0x0, x1x1, y0y0, y1y1) == 0) {
        //uniform quad, 2 bit code
        return 2;
    }
    int halfSize = imageSize >> 1;
    //Performance: parallel_for and non-parallel are the same
    // profiling showing overhead of concurrency is not worth it
    // probably due to 
    //  1. asymetry process time for each quad (more wait time)
    //  2. too many parallel (high overhead)
    return 1 +   //first 0
        codeLenR(x0, y0, n - 1, x0x0, -1, y0y0, -1) + //lower left quad
        codeLenR(x0, y0 + halfSize, n - 1, x0x0, -1, -1, y1y1) + //uper left quad
        codeLenR(x0 + halfSize, y0, n - 1, -1, x1x1, y0y0, -1) +   //lower right quad
        codeLenR(x0 + halfSize, y0 + halfSize, n - 1, -1, x1x1, -1, y1y1); //upper right quad
}

const int DIR[4][2] = { { 0, 0 },
                        { 0, 1 },
                        { 1, 1 },
                        { 1, 0 } };
int solve() {
    //we know first level is not uniform, 
    //and this first cut will ensure the assumption
    //about white block is correct (when look at the whole square, we will 
    //have four corners white while the region is still ununiform, but
    //once cut to quad, we can be sure the inside will all be white too)
    int halfSize = int(POWER2[N - 1]);

    //parallel here paid off, runtime improved from 2.5 sec to 1 sec 
    //probably due to similar cal time for each quad
    int arrLen[3];
    parallel_for(0, 3,
        [&](int i) {
        arrLen[i] = codeLenR(halfSize * DIR[i][0],
            halfSize * DIR[i][1], N - 1, 
            -1, -1, -1, -1);
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
