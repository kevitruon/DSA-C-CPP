/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   BigInteger.cpp
 ************************************************************/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <cstring>
#include "List.h"
#include "BigInteger.h"

const int power = 9;                 // log_10(base)
const ListElement base = 1000000000; // 10^(power)

// Helper functions -----------------------------------------------------------

// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List &L)
{
    L.moveFront();
    while (L.position() < L.length())
    {
        ListElement x = L.moveNext(); // set a variable "x" of type ListElement to get the data of the next element of L
        // if x is 0 then continue
        L.eraseBefore();
        // instead insert the same data "x" but negative
        L.insertBefore(-x);
    }
}

// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List &S, List A, List B, int sgn)
{
    ListElement x, y;
    // clear the state of "S"
    S.clear();
    // move to the front of A and B
    A.moveFront();
    B.moveFront();
    // iterate through the two lists A && B:
    while (A.position() < A.length() && B.position() < B.length())
    {
        // set "x" and "y" to get the data of the next element of "A" and "B"
        x = A.moveNext();
        y = B.moveNext();
        // in "S" insert (x plus y*sgn)
        S.insertBefore(x + (y * sgn));
    }
    // now iterate through list "A":
    while (A.position() < A.length())
    {
        // set "x" to get the data of the next element of "A"
        x = A.moveNext();
        // insert "x" in the stack
        S.insertBefore(x);
    }
    // now iterate through list "B":
    while (B.position() < B.length())
    {
        // set "y" to get the data of the next element of "B"
        y = B.moveNext();
        // insert "y*sgn" in the stack
        S.insertBefore(y * sgn);
    }  
}

// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List &L)
{
    int sgn;
    ListElement q, r, u;
    ListElement carryIn, curCol, carryOut = 0;

    L.moveFront();
    while (L.position() < L.length())
    {
        carryIn = carryOut;
        curCol = L.moveNext();
        curCol += carryIn;
        q = curCol / base;
        r = curCol % base;
        u = (r < 0) ? 1 : 0;

        curCol = r + u * base;
        carryOut = q - u;
        L.eraseBefore();
        L.insertBefore(curCol);

        if (L.position() == L.length())
        {
            if (carryOut > 0)
            {
                L.insertAfter(0);
            }
            else if (carryOut < 0)
            {
                L.insertAfter(carryOut);
                break;
            }
        }
    }

    // L.moveFront();
    // while (L.position() < L.length() && L.moveNext() == 0)
    // {
    //     L.eraseBefore();
    // }
    // if (L.length() != 0)
    // {
    //     L.moveFront();
    //     if (L.moveNext() > 0)
    //     {
    //         sgn = 1;
    //     }
    //     else
    //     {
    //         sgn = -1;
    //         negateList(L);
    //         normalizeList(L);
    //     }
    // }else{
    //     sgn =0;
    // }

    L.moveBack();
    while(L.position()!=0){
        if(L.movePrev() == 0){
            L.eraseAfter();
        }else{
            break;
        }
    }

    if (L.length() != 0)
    {
        L.moveBack();
        if (L.movePrev() > 0)
        {
            sgn = 1;
        }
        else
        {
            sgn = -1;
            negateList(L);
            normalizeList(L);
        }
    }
    else
    {
        sgn = 0;
    }

    return sgn;
}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List &L, int p)
{
    // move to the front of L
    L.moveFront();
    // iterate "i" from 0 to p (exclusive):
    for (int i = 0; i < p; i++)
    {
        // insert 0's after the cursor on L
        L.insertAfter((ListElement)0);
    }
}

// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List &L, ListElement m)
{
    // if "m" is not 1:
    if (m != 1)
    {
        L.moveFront();
        // move to the front of L
        // iterate through L:
        while (L.position() < L.length())
        {
            // set a variable "x" of type ListElement to get the data of the next element of L
            ListElement x = L.moveNext();
            // if x is 0 then continue
            // erase the element
            L.eraseBefore();
            // insert (x*m)
            L.insertBefore(x * m);
        }
    }
}

// Class Constructors & Destructors ----------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state:
// signum=0, digits=().
BigInteger::BigInteger()
{
    signum = 0;
    digits = List();
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s)
{
    if (s.size()==0)
    {
        std::cout << "BigInteger: Constructor: empty string";
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    if (s[0] == '-' || s[0] == '+')
    {
        if (s.size() == 1)
        {
            std::cout << "BigInteger: Constructor: non-numeric string";
            throw std::length_error("BigInteger: Constructor: non-numeric string");
        }
    }
    if (s[0] == '-')
    {
        signum = -1;
        s = s.substr(1);
    }
    else
    {
        signum = 1;
        if (s[0] == '+')
        {
            s = s.substr(1);
        }
    }
    while (s.size() > 1 && s[0] == '0')
    {
        s = s.substr(1);
    }
    digits.clear();
    if (s[0] == '0')
    {
        signum = 0;
    }
    else
    {
        int r = (s.size()) % power;
        for (int i = (s.size()) - power; i > -1; i -= power)
        {
            digits.insertBefore(std::stol(s.substr(i, power)));
        }
        if (r > 0)
        {
            digits.insertBefore(std::stol(s.substr(0, r)));
        }
    }
}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger &N)
{
    signum = N.signum;
    digits = N.digits;
}

// Optional Destuctor
// ~BigInteger()
// ~BigInteger();

// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is negative,
// positive or 0, respectively.
int BigInteger::sign() const
{
    return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger &N) const
{

    List L = this->digits;
    List R = N.digits;

    // BASE CASES:
    // check for signums and return appropriate value for "ret"
    // example:
    if (signum > N.signum)
    {
        return 1;
    }
    else if (signum < N.signum)
    {
        return -1;
    }
    // check for the lengths
    // example:
    if (L.length() < R.length())
    {
        return -1;
    }
    else if (L.length() > R.length())
    {
        return 1;
    }
    L.moveFront();
    R.moveFront();
    while (L.position() < L.length() && R.position() < R.length())
    {
        // then iterate through the two lists:
        // check the elements in the two lists and return appropriate value for "ret"
        if (L.moveNext() > R.moveNext())
        {
            return 1;
        }
        if (L.peekPrev() < R.peekPrev())
        {
            return -1;
        }
        // move next on the two lists
    }
    return 0;
}

// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero()
{
    digits.clear();
    signum = 0;
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of
// this BigInteger positive <--> negative.
void BigInteger::negate()
{
    signum = signum * (-1);
}

// BigInteger Arithmetic operations ----------------------------------------

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger &N) const
{
    BigInteger Sum;
    int sgn;
    List A = this->digits;
    List B = N.digits;
    List &s = Sum.digits;

    if (this->signum == N.signum)
    {
        sumList(s, A, B, 1);
        normalizeList(s);
        Sum.signum = this->signum;
    }
    else
    {
        sumList(s, A, B, -1);
        sgn = normalizeList(s);
        Sum.signum = sgn * this->signum;
    }

    return Sum;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger &N) const
{
    BigInteger A = BigInteger(N);
    // negate "A"
    A.negate();
    // call and return the add function on "A"
    return add(A);
}

// mult()
// Returns a BigInteger representing the product of this and N.
BigInteger BigInteger::mult(const BigInteger &N) const
{
    int col = 0; // column number
    BigInteger Prod;
    List A = this->digits;
    List B = N.digits;
    List C;

    List &P = Prod.digits;
    // set P's signum by multiplying the signs of A and B's signums
    Prod.signum = signum * N.signum;

    // move to the front of "B"
    B.moveFront();
    // iterate thriugh "B":
    while (B.position() < B.length())
    {
        // set C to A
        C = A;
        // set a variable "x" of type ListElement to get the data of the next element of B
        ListElement x = B.moveNext();
        // multiply list C by "x"
        scalarMultList(C, x);

        // normalize C by shifting the list by the number of columns "col"
        shiftList(C, col);
        // reassign P by adding the Lists C and P
        sumList(P, C, P, 1);
        // normalize P
        normalizeList(P);
        // increment the column number
        col++;
    }

    return Prod;
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string()
{
    ListElement x;
    std::string s;

    if (signum < 0)
    {
        s += '-';
    }
    if (signum == 0)
    {
        s += '0';
        return s;
    }
    List A = digits;
    std::ostringstream ss;
    A.moveBack();
    x = A.movePrev();
    ss << x;
    s += ss.str();
    while (A.position()>0)
    {
        x = A.movePrev();
        std::ostringstream ss;
        ss << std::setw(power) << std::setfill('0') << x;
        s += ss.str();
    }
    return s;
}

// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream &operator<<(std::ostream &stream, BigInteger N)
{
    return stream << N.BigInteger::to_string();
}

// operator==()
// Returns true if and only if A equals B.
bool operator==(const BigInteger &A, const BigInteger &B)
{
    return (A.compare(B)==0);
}

// operator<()
// Returns true if and only if A is less than B.
bool operator<(const BigInteger &A, const BigInteger &B)
{
    return (A.compare(B) == -1);
}

// operator<=()
// Returns true if and only if A is less than or equal to B.
bool operator<=(const BigInteger &A, const BigInteger &B)
{
    return (A.compare(B) == -1 || A == B);
}

// operator>()
// Returns true if and only if A is greater than B.
bool operator>(const BigInteger &A, const BigInteger &B)
{
    return (A.compare(B) == 1);
}

// operator>=()
// Returns true if and only if A is greater than or equal to B.
bool operator>=(const BigInteger &A, const BigInteger &B)
{
    return (A.compare(B) == 1 || A == B);
}

// operator+()
// Returns the sum A+B.
BigInteger operator+(const BigInteger &A, const BigInteger &B)
{
    BigInteger C = A;
    return C.add(B);
}

// operator+=()
// Overwrites A with the sum A+B.
BigInteger operator+=(BigInteger &A, const BigInteger &B)
{
    A=A+B;
    return A;
}

// operator-()
// Returns the difference A-B.
BigInteger operator-(const BigInteger &A, const BigInteger &B)
{
    BigInteger C = A;
    return C.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B.
BigInteger operator-=(BigInteger &A, const BigInteger &B)
{
    A=A-B;
    return A;
}

// operator*()
// Returns the product A*B.
BigInteger operator*(const BigInteger &A, const BigInteger &B)
{
    BigInteger C = A;
    return C.mult(B);
}

// operator*=()
// Overwrites A with the product A*B.
BigInteger operator*=(BigInteger &A, const BigInteger &B)
{
    A=A*B;
    return A;
}