/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Arithmetic.cpp
 ************************************************************/
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "BigInteger.h"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in;
    ofstream out;
    string A_Str, blank, B_Str;

    // check command line for correct number of arguments
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return (EXIT_FAILURE);
    }

    // open files for reading and writing
    in.open(argv[1]);
    if (!in.is_open())
    {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return (EXIT_FAILURE);
    }

    out.open(argv[2]);
    if (!out.is_open())
    {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return (EXIT_FAILURE);
    }

    // Pull lines from in file
    getline(in, A_Str);
    getline(in, blank);
    getline(in, B_Str);

    // Parse input strings as BigIntegers
    BigInteger A(A_Str);                   // A
    BigInteger B(B_Str);                   // B
    BigInteger first = A + B;              // A+B
    BigInteger second = A - B;             // A-B
    BigInteger third = A - A;              // A-A
    BigInteger fourth = A + A + A - B - B; // 3A-2B
    BigInteger fifth = A * B;              // AB
    BigInteger sixth = A * A;              // A^2
    BigInteger seventh = B * B;            // B^2

    BigInteger eigthA = sixth * sixth;
    BigInteger eigthB = seventh * seventh * B;
    BigInteger eigth = eigthA + eigthA + eigthA + eigthA + eigthA 
    + eigthA + eigthA + eigthA + eigthA + eigthB + eigthB + eigthB 
    + eigthB + eigthB + eigthB + eigthB + eigthB + eigthB + eigthB 
    + eigthB + eigthB + eigthB + eigthB + eigthB + eigthB; // 9A^4+16B^5

    // Write output to file
    out << A << endl
        << endl;
    out << B << endl
        << endl;
    out << first << endl
        << endl;
    out << second << endl
        << endl;
    out << third << endl
        << endl;
    out << fourth << endl
        << endl;
    out << fifth << endl
        << endl;
    out << sixth << endl
        << endl;
    out << seventh << endl
        << endl;
    out << eigth << endl
        << endl;

    // close files
    in.close();
    out.close();

    return (EXIT_SUCCESS);
}
