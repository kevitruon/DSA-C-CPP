/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Shuffle.cpp
 ************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include "List.h"

void shuffle(List &D)
{
    List left, right;
    int size = D.length();

    if (size % 2 == 0)
    {
        D.moveFront();
        for (int i = 0; i < size / 2; i++)
        {
            left.insertBefore(D.moveNext());
        }
        while (D.position() < size)
        {
            right.insertBefore(D.moveNext());
        }
    }
    else
    {
        D.moveFront();
        for (int i = 0; i < size / 2; i++)
        {
            left.insertBefore(D.moveNext());
        }
        while (D.position() < size)
        {
            right.insertBefore(D.moveNext());
        }
    }

    D.clear();
    left.moveFront();
    right.moveFront();
    for (int i = 0; i < size / 2; i++)
    {
        D.insertBefore(right.moveNext());
        D.insertBefore(left.moveNext());
    }
    if (size % 2 != 0)
    {
        D.insertBefore(right.moveNext());
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " MAX_DECK_SIZE\n";
        return 1;
    }
    int maxDeckSize = std::stoi(argv[1]);

    std::cout << "deck size shuffle count\n";
    std::cout << "------------------------------\n";
    for (int n = 1; n <= maxDeckSize; n++)
    {
        List deck;
        List comp;
        for (int i = 0; i < n; i++)
        {
            deck.insertBefore(i);
            comp.insertBefore(i);
        }
        shuffle(deck);
        int count = 1;
        while (!deck.equals(comp))
        {
            shuffle(deck);
            count++;
        }
        std::cout << n << " " << count << std::endl;
    }
    return 0;
}
