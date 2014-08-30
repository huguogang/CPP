// EulerProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <set>

using namespace std;

template<class t> void printVector(vector<t>& v)
{
    for (vector<t>::const_iterator i = v.begin(); i != v.end(); ++i)
    {
        cout << *i << endl;
    }
}

/*
const int nDigits = 5;
const int nGuesses = 6;
int guesses[nGuesses][nDigits] =
{
    { 9, 0, 3, 4, 2 },
    { 7, 0, 7, 9, 4 },
    { 3, 9, 4, 5, 8 },
    { 3, 4, 1, 0, 9 },
    { 5, 1, 5, 4, 5 },
    { 1, 2, 5, 3, 1 }
};
int matches[nGuesses] = { 2, 0, 2, 1, 2, 1 };
//*/
///* big data set
const int nDigits = 16;
const int nGuesses = 22;
int guesses[nGuesses][nDigits] =
{
{ 5, 6, 1, 6, 1, 8, 5, 6, 5, 0, 5, 1, 8, 2, 9, 3 }, 
{ 3, 8, 4, 7, 4, 3, 9, 6, 4, 7, 2, 9, 3, 0, 4, 7 },
{ 5, 8, 5, 5, 4, 6, 2, 9, 4, 0, 8, 1, 0, 5, 8, 7 }, 
{ 9, 7, 4, 2, 8, 5, 5, 5, 0, 7, 0, 6, 8, 3, 5, 3 },
{ 4, 2, 9, 6, 8, 4, 9, 6, 4, 3, 6, 0, 7, 5, 4, 3 }, 
{ 3, 1, 7, 4, 2, 4, 8, 4, 3, 9, 4, 6, 5, 8, 5, 8 },
{ 4, 5, 1, 3, 5, 5, 9, 0, 9, 4, 1, 4, 6, 1, 1, 7 }, 
{ 7, 8, 9, 0, 9, 7, 1, 5, 4, 8, 9, 0, 8, 0, 6, 7 },
{ 8, 1, 5, 7, 3, 5, 6, 3, 4, 4, 1, 1, 8, 4, 8, 3 },
{ 2, 6, 1, 5, 2, 5, 0, 7, 4, 4, 3, 8, 6, 8, 9, 9 },
{ 8, 6, 9, 0, 0, 9, 5, 8, 5, 1, 5, 2, 6, 2, 5, 4 },
{ 6, 3, 7, 5, 7, 1, 1, 9, 1, 5, 0, 7, 7, 0, 5, 0 },
{ 6, 9, 1, 3, 8, 5, 9, 1, 7, 3, 1, 2, 1, 3, 6, 0 },
{ 6, 4, 4, 2, 8, 8, 9, 0, 5, 5, 0, 4, 2, 7, 6, 8 },
{ 2, 3, 2, 1, 3, 8, 6, 1, 0, 4, 3, 0, 3, 8, 4, 5 }, 
{ 2, 3, 2, 6, 5, 0, 9, 4, 7, 1, 2, 7, 1, 4, 4, 8 },
{ 5, 2, 5, 1, 5, 8, 3, 3, 7, 9, 6, 4, 4, 3, 2, 2 }, 
{ 1, 7, 4, 8, 2, 7, 0, 4, 7, 6, 7, 5, 8, 2, 7, 6 },
{ 4, 8, 9, 5, 7, 2, 2, 6, 5, 2, 1, 9, 0, 3, 0, 6 },
{ 3, 0, 4, 1, 6, 3, 1, 1, 1, 7, 2, 2, 4, 6, 3, 5 },
{ 1, 8, 4, 1, 2, 3, 6, 4, 5, 4, 3, 2, 4, 5, 8, 9 }, 
{ 2, 6, 5, 9, 8, 6, 2, 6, 3, 7, 3, 1, 6, 8, 6, 7 }
};
int matches[nGuesses] = { 2, 1, 3, 3, 3, 1, 2, 3, 1, 2, 3, 1, 1, 2, 0, 2, 2, 3, 1, 3, 3, 2 };

//*/

int totalMatches = 0; //total number of matches needed
vector<map<int, vector<int>>> allCandidates;
int arrReachableMatchCount[nDigits + 1][100];

void preprocess() {
    //extract candidates for each digit
    for (int digit = 0; digit < nDigits; digit++) {
        //-- identify all candidates of the current digit
        //map
        //  key: all unique numbers guessed for this digit
        //  val: index of all guesses that has the number at this digit

        map<int, vector<int>> candidates;
        for (int i = 0; i < nGuesses; i++) {
            int n = guesses[i][digit];
            if (candidates.find(n) == candidates.end()) {
                candidates[n] = vector<int>();
            }
            candidates[n].push_back(i);
        }
        if (candidates.size() == 9) {
            //corner cutting:
            //since we know there is unique answer, if there is only one digit that is not guessed
            //then it might be the right number, otherwise, they should not be
            int missingNumber;
            for (missingNumber = 0; missingNumber <= 9; missingNumber++) {
                if (candidates.find(missingNumber) == candidates.end()) {
                    break;
                }
            }
            candidates[missingNumber] = vector<int>(0); //size 0 entry
        }
        allCandidates.push_back(candidates);
    }
    
    //all match counts that can be reached at certain digit
    vector<set<int>> reachableMatchCount(nDigits + 1);
    reachableMatchCount[nDigits].insert(0); //boundary condition
    for (int digit = nDigits - 1; digit >= 0; digit--) {
        set<int> matchCount; //counts of this digit        
        for (auto it = allCandidates[digit].begin(); it != allCandidates[digit].end(); ++it) {
            matchCount.insert(it->second.size());
        }
        //cross product to find all reachable digits
        for (auto it1 = reachableMatchCount[digit + 1].begin();
            it1 != reachableMatchCount[digit + 1].end(); ++it1) {
            for (auto it2 = matchCount.begin(); it2 != matchCount.end(); ++it2) {
                reachableMatchCount[digit].insert(*it1 + *it2);
            }
        }
    }
    int i = 0;
    int j = 0;
    for (auto digit = reachableMatchCount.begin(); digit != reachableMatchCount.end(); ++digit) {
        for (auto val = digit->begin(); val != digit->end(); ++val) {
            arrReachableMatchCount[i][*val] = 1;
            ++j;
        }
        ++i;
    }

    for (int i = 0; i< nGuesses; ++i) {
        totalMatches += matches[i];
    }
}
int answer[nDigits];

int loopCount = 0;
int maxLoop = 100000000;//answer if found at 50 mil;
int matchesSoFar = 0; //total matches found so far in the search
int maxMatchCount = 0; //just curious, how helpful is this pruning
bool searchR(int digit, int matches[], const int guesses[nGuesses][nDigits])
{
    loopCount++;
    //termination: dead looping
    if (loopCount > maxLoop) {
        cout << "abort, too many loops" << endl;
        for (int i = 0; i < nDigits; ++i) {
            cout << answer[i] << ", ";
        }
        cout << endl;
        return false;
    }

    //termination: last digit
    if (digit >= nDigits) {
        bool ret = true;
        for (int i = 0; i < nGuesses; i++) {
            if (matches[i] > 0) {
                ret = false;
                break;
            }
            else if (matches[i] < 0) {
                cout << "Error: matches should not be negative. i: " << i << endl;
            }
        }
        return ret;
    }
    
    //exclude matches == 0 numbers in the round
    int mask[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int maxMatch = nDigits - digit;
    for (int i = 0; i < nGuesses; i++) {
        if (matches[i] == 0) {
            mask[guesses[i][digit]] = 0;
        }
        //each guess should have number of matching digits no greater than remaining number of digits
        if (matches[i] > maxMatch) {
            ++maxMatchCount;
            return false;
        }
    }

    //search through each candidate recursively
    for (auto iCandidate = allCandidates[digit].begin(); iCandidate != allCandidates[digit].end(); ++iCandidate) {
        int number = iCandidate->first;
        if (mask[number] == 0) {
            //hit a 0 match number exclusion
            continue;
        }
        //record this search
        vector<int>* myGuesses = &iCandidate->second;
        answer[digit] = number;
        matchesSoFar += myGuesses->size();
        

        //total number of match can be reached
        if (arrReachableMatchCount[digit + 1][totalMatches - matchesSoFar] != 0) {
            for (auto itGuess = myGuesses->begin(); itGuess != myGuesses->end(); ++itGuess) {
                int idx = *itGuess;
                --matches[idx];
            }
        
            if (searchR(digit + 1, matches, guesses)) {
                return true;
            }
            for (auto itGuess = myGuesses->begin(); itGuess != myGuesses->end(); ++itGuess) {
                int idx = *itGuess;
                ++matches[idx];
            }
        }

        //rewind
        matchesSoFar -= myGuesses->size();
    }
    //all searches failed to find an answer
    return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
    clock_t c1 = clock();
    preprocess();
    bool b = searchR(0, matches, guesses);
    clock_t c2 = clock();
    cout << "Total seconds: " << (c2 - c1)/CLOCKS_PER_SEC << endl;
    cout << "Loop count: " << loopCount << endl;
    cout << "Max Match rule exclusion count: " << maxMatchCount << endl;
    if (b) {
        for (int i = 0; i < nDigits; ++i) {
            cout << answer[i] << ", ";
        }
        cout << endl;
    }
    else {
        cout << "Failed to find any answer" << endl;
    }
    //cin.ignore(1);
    return 0;
}
