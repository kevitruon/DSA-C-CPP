/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Graph.h
 ************************************************************/
#include <stdbool.h>
#include "List.h"

#define UNDEF -3
#define INF -2
#define NIL -1
#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct GraphObj *Graph;

// Constructors-Destructors
Graph newGraph(int n);
void freeGraph(Graph *pG);
// Access functions
int getOrder(Graph G);
int getSize(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getDist(Graph G, int u);
int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u);   /* Pre: 1<=u<=n=getOrder(G) */
void getPath(List L, Graph G, int u);
// Manipulation procedures
void makeNull(Graph G);
void BFS(Graph G, int s);
void addArc(Graph G, int u, int v);  /* Pre: 1<=u<=n, 1<=v<=n */
void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void DFS(Graph G, List S);           /* Pre: length(S)==getOrder(G) */
// Other Functions
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE *out, Graph G);

// Helper Functions
void Visit(Graph G, List S, int *time, int vert);