#include "stdafx.h"

#include <stdio.h>
#include <assert.h>
#include <string>
#include <iostream>

#include "BigInteger.h"

ostream& operator << (ostream& s, const BigInteger &big) {
    if (big.data.size() == 0) {
        s << 0;
    }
    else {
        for (int i = big.data.size() - 1; i >= 0; i--) {
            s << int(big.data[i]);
        }
    }
    return s;
}

BigInteger::BigInteger() {
}

BigInteger::BigInteger(const BigInteger &other) {
    data = other.data;
}

BigInteger::BigInteger(int other) {
    assert(other >= 0);

    while (other > 0) {
        data.push_back(other % 10);
        other = other / 10; //by definition, this floor
    }
}

BigInteger::BigInteger(string value) {
    //TODO: init with string value
    unsigned char zero = '0';
    for (int i = value.length() - 1; i >= 0; i--) {
        data.push_back(value[i] - zero);
    }
}
void BigInteger::Clear() {
    data.clear();
}

void BigInteger::Copy(const BigInteger &rhs) {
    if (this == &rhs) {
        return;
    }

    Clear();
    data = rhs.data;
}

void BigInteger::Add(const BigInteger &rhs) {
    assert(this != &rhs); //NYI: add itself
    int rLen = rhs.GetSize();
    int carry = 0;
    for (int rIdx = 0; rIdx < rLen; rIdx++) {
        carry = AddWithCarry(rIdx, rhs.data[rIdx], carry);
    }
    int idx = rLen;
    while (carry > 0) {
        carry = AddWithCarry(idx, 0, carry);
        idx++;
    }
}

int BigInteger::GetSize() const{
    return data.size();
}


BigInteger& BigInteger::operator=(const BigInteger &rhs) {
    if (this != &rhs) {
        data.clear();
        data = rhs.data;
    }
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger &rhs) {
    Add(rhs);
    return *this;
}

const BigInteger BigInteger::operator +(const BigInteger &other) const {
    BigInteger ret = *this;
    ret += other;
    return ret;
}

unsigned char BigInteger::AddWithCarry(int place, unsigned char rhs, unsigned char carry) {
    if (GetSize() <= place) {
        data.push_back(0);
    }
    int val = data[place] + rhs + carry;
    data[place] = val % MAX_DIGIT;
    return val / MAX_DIGIT;
}