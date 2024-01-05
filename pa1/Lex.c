/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Lex.c
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "List.h"

#define MAX 300

int main(int argc, char *argv[])
{
    FILE *in, *out;
    char line[MAX];
    if (argc != 3)
    {
        printf("Incorrect input use: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (in == NULL)
    {
        printf("File: %s is unreadable\n", argv[1]);
        exit(1);
    }
    if (out == NULL)
    {
        printf("File: %s is unwritable\n", argv[1]);
        exit(1);
    }

    char maxInput[MAX][MAX];
    int i = 0;
    while (fgets(line, MAX, in) != NULL)
    {
        strcpy(maxInput[i], strtok(line, "\n"));
        i++;
    }

    char word[MAX][MAX];
    for (int j = 0; j < i; j++)
    {
        strcpy(word[j], maxInput[j]);
    }
    List order = newList();
    if(length(order) == 0)
        insertAfter(order,0);
    else
        insertAfter(order, length(order) - 1);
    int size = i;
    for (int i = 1; i < size; i++) {
        char key[MAX];
        strcpy(key, word[i]);
        moveBack(order);
        int j = length(order) - 1;
        while (j >= 0 && strcmp(word[get(order)], key) > 0) {
            movePrev(order);
            j--;
        }
        if (j == -1) {
            insertBefore(order, i);
        } else {
            insertAfter(order, i);
        }
    }
    moveFront(order);
    for (int j = 0; j < size; j++)
    {
        fprintf(out, "%s", word[get(order)]);
        moveNext(order);
    }
    freeList(&order);
    fclose(in);
    fclose(out);
    return 0;
}

    