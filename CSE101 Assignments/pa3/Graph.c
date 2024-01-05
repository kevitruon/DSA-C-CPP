/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Graph.c
 ************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "List.h"
#include "Graph.h"

/*** Structures ***/
typedef struct GraphObj
{
    List *neighbor;
    int *color;
    int *parent;
    int *distance;
    int order;
    int size;
    int source;
    int *disc;
    int *fin;
} GraphObj;

/*** Constructors-Destructors ***/
Graph newGraph(int n)
{
    Graph G = malloc(sizeof(GraphObj));
    G->neighbor = calloc(n + 1, sizeof(List));
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->distance = calloc(n + 1, sizeof(int));
    G->disc = calloc(n + 1, sizeof(int));
    G->fin = calloc(n + 1, sizeof(int));
    G->order = n;
    G->size = 0;
    G->source = NIL;
    for (int i = 1; i <= n; i++)
    {
        G->neighbor[i] = newList();
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->distance[i] = INF;
        G->disc[i] = UNDEF;
        G->fin[i] = UNDEF;
    }
    return G;
}
void freeGraph(Graph *pG)
{
    for (int i = 1; i <= getOrder(*pG); i++)
    {
        freeList(&(*pG)->neighbor[i]);
    }
    free((*pG)->neighbor);
    free((*pG)->color);
    free((*pG)->parent);
    free((*pG)->distance);
    free((*pG)->disc);
    free((*pG)->fin);
    free(*pG);
    *pG = NULL;
}
/*** Access functions ***/
int getOrder(Graph G)
{
    return G->order;
}
int getSize(Graph G)
{
    return G->size;
}
int getSource(Graph G)
{
    return G->source;
}
int getParent(Graph G, int u)
{
    if (u < 1 || u > getOrder(G))
    {
        printf("Graph Error: Calling getParent with invalid Vertex");
        exit(1);
    }
    return G->parent[u];
}
int getDist(Graph G, int u)
{
    if (u < 1 || u > getOrder(G))
    {
        printf("Graph Error: Calling getDist with invalid Vertex");
        exit(1);
    }
    return G->distance[u];
}
void getPath(List L, Graph G, int u)
{
    if (getSource(G) == NIL)
    {
        printf("Graph Error: BFS() must be called before getPath()\n");
        exit(1);
    }
    if (u < 1 || u > getOrder(G) || u == NIL)
    {
        printf("Graph Error: getPath() called on invalid vertex\n");
        exit(1);
    }
    if (u == getSource(G))
    {
        append(L, u);
    }
    else if (getParent(G, u) == NIL)
    {
        append(L, NIL);
    }
    else
    {
        getPath(L, G, getParent(G, u));
        append(L, u);
    }
}
int getDiscover(Graph G, int u)
{
    if (u < 1 || u > getOrder(G))
    {
        printf("Graph Error: Calling getDiscover with invalid Vertex");
        exit(1);
    }
    return G->disc[u];
}
int getFinish(Graph G, int u)
{
    if (u < 1 || u > getOrder(G))
    {
        printf("Graph Error: Calling getFinish with invalid Vertex");
        exit(1);
    }
    return G->fin[u];
}
/*** Manipulation procedures ***/
void makeNull(Graph G)
{
    if (G == NULL)
    {
        printf("Graph Error: Calling makeNULL on NULL Graph\n");
        exit(1);
    }
    for (int i = 1; i <= getOrder(G); i++)
    {
        clear(G->neighbor[i]);
    }
    G->size = 0;
    G->source = NIL;
}
void addEdge(Graph G, int u, int v)
{
    addArc(G, u, v);
    addArc(G, v, u);
    G->size--;
}
void addArc(Graph G, int u, int v)
{
    if (u < 1 || u > getOrder(G))
    {
        printf("Graph Error: Vertices1 is out of bounds\n");
        exit(1);
    }
    if (v < 1 || v > getOrder(G))
    {
        printf("Graph Error: Vertices2 out of bounds\n");
        exit(1);
    }
    if (length(G->neighbor[u]) == 0)
    {
        append(G->neighbor[u], v);
        G->size++;
        return;
    }
    else
    {
        for (moveFront(G->neighbor[u]); index(G->neighbor[u]) != -1; moveNext(G->neighbor[u]))
        {
            if (get(G->neighbor[u]) == v)
            {
                break;
            }
            if (get(G->neighbor[u]) > v)
            {
                insertBefore(G->neighbor[u], v);
                G->size++;
                break;
            }
        }
        if (index(G->neighbor[u]) == -1)
        {

            append(G->neighbor[u], v);
            G->size++;
        }
    }
}
void BFS(Graph G, int s)
{
    for (int i = 1; i <= G->order; i++)
    {
        G->color[i] = WHITE;
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }
    G->source = s;
    G->color[s] = GRAY;
    G->distance[s] = 0;
    G->parent[s] = NIL;
    List queue = newList();
    append(queue, s);
    while (length(queue) > 0)
    {
        int u = front(queue);
        deleteFront(queue);
        for (int i = 0; index(G->neighbor[u]) >= 0; i++)
        {
            int v = get(G->neighbor[u]);
            moveNext(G->neighbor[u]);
            if (G->color[v] == WHITE)
            {
                G->color[v] = GRAY;
                G->distance[v] = G->distance[u] + 1;
                G->parent[v] = u;
                append(queue, v);
            }
        }
        G->color[u] = BLACK;
    }
    freeList(&queue);
}
void DFS(Graph G, List S)
{
    for (int i = 1; i <= G->order; i++)
    {
        G->color[i] = WHITE;
        G->parent[i] = NIL;
    }
    int time = 0;
    moveFront(S);
    while (index(S) >= 0)
    {
        int y = get(S);
        if (G->color[y] == WHITE)
        {
            Visit(G, S, &time, y);
        }
        moveNext(S);
    }
    for (int i = 0; i < getOrder(G); i++)
    {
        deleteBack(S);
    }
}

/*** Other operations ***/
void printGraph(FILE *out, Graph G)
{
    for (int i = 1; i <= getOrder(G); i++)
    {
        fprintf(out, "%d: ", i);
        printList(out, G->neighbor[i]);
        fprintf(out, "\n");
    }
}
Graph transpose(Graph G)
{
    Graph gT = newGraph(G->order);
    for (int i = 1; i <= getOrder(G); i++)
    {
        for (moveFront(G->neighbor[i]); index(G->neighbor[i]) != NIL; moveNext(G->neighbor[i]))
        {
            int vert = get(G->neighbor[i]);
            addArc(gT, vert, i);
        }
    }
    return gT;
}
Graph copyGraph(Graph G)
{
    Graph cpyG = newGraph(G->order);
    for (int i = 1; i <= G->order; i++)
    {
        for (moveFront(G->neighbor[i]); index(G->neighbor[i]) != NIL; moveNext(G->neighbor[i]))
        {
            int vert = get(G->neighbor[i]);
            addArc(cpyG, i, vert);
        }
    }
    return cpyG;
}

/*** Helper Operations ***/
void Visit(Graph G, List S, int *time, int vert)
{
    G->disc[vert] = ++(*time);
    G->color[vert] = GRAY;
    moveFront(G->neighbor[vert]);
    while (index(G->neighbor[vert]) >= 0)
    {
        int y = get(G->neighbor[vert]);
        if (G->color[y] == WHITE)
        {
            G->parent[y] = vert;
            Visit(G, S, time, y);
        }
        moveNext(G->neighbor[vert]);
    }
    G->color[vert] = BLACK;
    G->fin[vert] = ++(*time);
    prepend(S, vert);
}
