//reusable math utilties
#include "stdafx.h"
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

//total number of factors of n (including 1, and n)
int GetFactorCount(long long n) {
    int count = 1;
    //n is a moving target in this loop
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

//check if n is prime
bool isPrime(int n) {
    if (n == 2) { 
        return true; 
    }
    if (n == 1 || (n%2 == 0)) {
        return false;
    }
    for (int i = 3; i <= sqrt(double(n)); i+=2) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}

//TODO: avoid copy constructor of pair, and vector for performance
//for n >= 2, find prime factors of n
void factorization(int n, vector<pair<int, int>>& out) {
    out.clear();
    for (int i = 2; i <= n; i++) {
        int count = 0;
        while (n%i == 0) {
            count++;
            n = n / i;
        }
        if (count > 0) {
            out.push_back(pair<int, int>(i, count));
        }
    }
}

void factorization(int n, const vector<int>& allPrimeNumbers, vector<pair<int, int>>& out) {
    out.clear();
    for (auto it = allPrimeNumbers.begin();
        it != allPrimeNumbers.end() && *it <=n; it++) {
        int count = 0;
        while (n%*it == 0) {
            count++;
            n = n / *it;
        }
        if (count > 0) {
            out.push_back(pair<int, int>(*it, count));
        }
    }
}

//find all proper divisors of integer n
void getAllProperDivisors(int n, vector<int> &out) {
    out.clear();
}

int totient(const vector<pair<int, int>> &factors) {
    int ret = 1;
    for (auto it = factors.begin(); it != factors.end(); it++) {
        int tmp = int(pow(it->first, it->second - 1));
        tmp = tmp * it->first - tmp;
        ret *= tmp;
    }
    return ret;
}

//calculate totient (phi) of integer n
// for a  n = p1^a1 + p2^a2 + ... pk^ak
//      phi(n) = (p1^a1 - p1^(a1 - 1)) * ... * (pk^ak - pk^(ak - 1))
int totient(int n) {
    vector<pair<int, int>> factors;
    factorization(n, factors);
    return totient(factors);
}

void getPrimeNumbers(int n, vector<int> &out) {
    out.clear();
    int s = int(sqrt(n));
    //assuming n is under 10 million
    //sift out non-prime
    vector<int> allNumbers(n + 1);
    for (int i = 2; i <= s; i++) {
        if (i > 2 && i % 2) {
            continue; //skip even numbers after 2
        }
        if (allNumbers[i] == 0) {
            //it is prime number
            int sift = i * i; //any smaller multiples should already been screened out
            while (sift <= n) {
                allNumbers[sift] = 1; //mark as non-prime
                sift += i;
            }
            out.push_back(i);
        }
    }
    for (int i = s + 1; i <= n; i++) {
        if (allNumbers[i] == 0) {
            out.push_back(i);
        }
    }
}

