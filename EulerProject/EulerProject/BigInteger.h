#pragma once

#include <istream>
#include <vector>
#include <string>
using namespace std;

class BigInteger {
public:
    BigInteger();
    BigInteger(const BigInteger &other);
    BigInteger(int other);
    BigInteger(string value);

    BigInteger& operator =(const BigInteger &rhs);
    
    BigInteger& operator +=(const BigInteger &rhs);
    //BigInteger& operator -=(const BigInteger &rhs);
    //BigInteger& operator *=(const BigInteger &rhs);
    //BigInteger& operator %=(const BigInteger &rhs);

    const BigInteger operator +(const BigInteger &other) const;
    //const BigInteger operator -(const BigInteger &other) const;
    //const BigInteger operator *(const BigInteger &other) const;
    //const BigInteger operator %(const BigInteger &other) const;

    //bool operator == (const BigInteger &other) const;
    //bool operator != (const BigInteger &other) const;

    void Clear();
    void Copy(const BigInteger &rhs);
    void Add(const BigInteger &rhs);
    int GetSize() const;
    //void Minus(const BigInteger &rhs);
    //void Multiply(const BigInteger &rhs);
    //void Mod(const BigInteger &rhs);

private:
    //NYI: negative number is not supported
    bool isNegative = false;
    //index 0 is least significant digit
    vector<unsigned char> data;

    //carry to next digit if result not less than this
    //10 is easier for decimal serialization and deserialization
    const int MAX_DIGIT = 10; 

    unsigned char AddWithCarry(int place, unsigned char rhs, unsigned char carry);

    friend ostream& operator << (ostream& s, const BigInteger &big);
};
//istream& operator >> (istream& s, BigInteger &big) {
//    return s;
//}

//TODO: 
// - prime test
// - factor
// - exponential
// - exponential mod
// - GCD
//http://dl.fefe.de/bignum.pdf
