/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   FindComponents.c
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "List.h"

int main(int argc, char *argv[])
{

    // needed variables
    int n, u, v, comp; // n = #vertices; u, v = vertex of two edges, comp = # of components
    FILE *in, *out;    // input output files
    Graph G, T;
    List S;

    // check command line for correct number of arguments
    if (argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // open files for reading and writing
    in = fopen(argv[1], "r");
    if (in == NULL)
    {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if (out == NULL)
    {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    // scan the first line or first digit to get # vertex
    fscanf(in, " %d", &n);
    // initialize a new graph with "n"
    // initialize u and v to 1 temporarly
    G = newGraph(n);
    u = v = 1;

    // while u or v are not 0:
    // scan u and v
    // check if either are 0 and break
    // call addArc on u and v
    while (u != 0 || v != 0)
    {
        fscanf(in, "%d %d", &u, &v);
        if (u == 0 || v == 0)
        {
            break;
        }
        addArc(G, u, v);
    }
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");

    S = newList();
    for (int i = 1; i <= n; i++)
    {
        append(S, i);
    }
    T = transpose(G);
    DFS(G, S);
    DFS(T, S);

    comp = 0;
    for (moveBack(S); index(S) != -1; movePrev(S))
    {
        if (getParent(T, get(S)) == NIL)
        {
            comp++;
        }
    }
    fprintf(out, "G contains %d strongly connected components:\n", comp);

    List *SCC = calloc(comp + 1, sizeof(List));
    for (int i = 1; i <= comp; i++)
    {
        SCC[i] = newList();
    }
    int comp2 = 1;
    for (moveBack(S); index(S) != -1; movePrev(S))
    {
        prepend(SCC[comp2], get(S));
        if (getParent(T, get(S)) == NIL)
        {
            fprintf(out, "Component %d: ", comp2);
            printList(out, SCC[comp2]);
            fprintf(out, "\n");
            comp2++;
        }
    }

    freeList(SCC);
    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    fclose(in);
    fclose(out);
    return (0);
}
