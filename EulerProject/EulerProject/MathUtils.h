#pragma once
#include <vector>
using namespace std;
//count number of factors of n
int GetFactorCount(long long n);
//check if n is prime number
bool isPrime(int n);
//decompose n into exponential of prime numbers
void factorization(int n, vector<pair<int, int>>& out);
void factorization(int n, const vector<int>& allPrimeNumbers, vector<pair<int, int>>& out);
//find all proper divisors of n >= 2
void getAllProperDivisors(int n, vector<pair<int, int>>& out);
//get totient number for n
int totient(int n);
int totient(const vector<pair<int, int>> &factors);
//find all prime numbers in [2, n]
void getPrimeNumbers(int n, vector<int>&out);