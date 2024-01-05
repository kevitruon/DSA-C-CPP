/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Order.cpp
 ************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Dictionary.h"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in;
    ofstream out;
    string line, inOrder, preOrder;
    int line_count;

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

    Dictionary D = Dictionary();
    // Pull lines from in file
    line_count = 0;
    while (getline(in, line))
    {
        line_count++;
        D.setValue(line, line_count);
    }
    inOrder = D.to_string();
    preOrder = D.pre_string();

    out << inOrder << endl;
    out << preOrder << endl;

    // close files
    in.close();
    out.close();

    return (EXIT_SUCCESS);
}
