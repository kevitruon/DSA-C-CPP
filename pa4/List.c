/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   List.c
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj *Node;

// private NodeObj type
typedef struct NodeObj
{
    void *data;
    Node next;
    Node prev;
} NodeObj;

// private ListObj type
typedef struct ListObj
{
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

Node newNode(void *data)
{
    Node N = malloc(sizeof(NodeObj) + sizeof(data));
    N->data = data;
    N->next = NULL;
    N->prev = NULL;
    return (N);
}

void freeNode(Node *pN)
{
    if (pN != NULL && *pN != NULL)
    {
        free(*pN);
        *pN = NULL;
    }
}

List newList(void)
{
    List L = malloc(sizeof(ListObj));
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return (L);
}

void freeList(List *pL)
{
    if (pL != NULL && *pL != NULL)
    {
        while (!isEmpty(*pL))
        {
            deleteFront(*pL);
        }
        free(*pL);
        pL = NULL;
    }
}

// Access functions -----------------------------------------------------------

int length(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    return (L->length);
}
// Returns index of cursor element if defined, -1 otherwise.
int index(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) <= 0)
    {
        return -1;
    }
    return (L->index);
}

// Returns front element of L. Pre: length()>0
void *front(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L))
    {
        printf("List Error: calling front() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    return (L->front->data);
}

// Returns back element of L. Pre: length()>0
void *back(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L))
    {
        printf("List Error: calling back() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    return (L->back->data);
}

// Returns cursor element of L. Pre: length()>0, index()>=0
void *get(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L))
    {
        printf("List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0)
    {
        printf("List Error: calling get() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    return (L->cursor->data);
}

// Manipulation procedures ----------------------------------------------------
// Resets L to its original empty state.
void clear(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    while (L->back != NULL)
    {
        deleteBack(L);
    }
}

// Overwrites the cursor element’s data with data.
// Pre: length()>0, index()>=0
void set(List L, void *data)
{
    if (L == NULL)
    {
        printf("List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L))
    {
        printf("List Error: calling set() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (L->index < 0)
    {
        printf("List Error: calling set() with cursor off the List\n");
        exit(EXIT_FAILURE);
    }
    L->cursor->data = data;
}

// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->length > 0)
    {
        L->index = 0;
        L->cursor = L->front;
    }
}
// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->length > 0)
    {
        L->index = L->length - 1;
        L->cursor = L->back;
    }
}

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0)
    {
        printf("List Error: calling movePrev() on empty List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->index == -1)
    {
        printf("List Error: calling movePrev() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    L->index--;
    L->cursor = L->cursor->prev;
}

// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor == L->back)
    {
        L->index = -1;
        L->cursor = NULL;
        return;
    }
    L->index++;
    L->cursor = L->cursor->next;
}
// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, void *data)
{
    if (L == NULL)
    {
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    else if (L->length == 0)
    {
        L->front = L->back = newNode(data);
    }
    else if (L->length == 1)
    {
        L->front = newNode(data);
        L->front->next = L->back;
        L->back->prev = L->front;
    }
    else
    {
        Node tempNode = L->front;
        L->front = newNode(data);
        L->front->next = tempNode;
        tempNode->prev = L->front;
    }
    L->length++;
    if (L->index != -1)
    {
        L->index++;
    }
}
// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, void *data)
{
    if (L == NULL)
    {
        printf("List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0)
    {
        L->front = newNode(data);
        L->back = L->front;
    }
    else if (L->length == 1)
    {
        L->back = newNode(data);
        L->front->next = L->back;
        L->back->prev = L->front;
    }
    else
    {
        Node tempNode = L->back;
        Node appNode = newNode(data);
        tempNode->next = appNode;
        appNode->prev = tempNode;
        L->back = appNode;
    }
    L->length++;
}
void insertBefore(List L, void *data)
{
    if (L == NULL)
    {
        printf("List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->index == -1)
    {
        printf("List Error: calling insertBefore() with out of index cursor\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor == NULL)
    {
        printf("List Error: calling insertBefore() on undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    Node insertNode = newNode(data);
    Node N = L->back;
    if (L->index == 0 && L->length == 1)
    {
        L->front = insertNode;
        insertNode->next = N;
        L->length++;
    }
    else if (L->index == 0)
    {
        L->front = insertNode;
        insertNode->next = L->cursor;
        L->cursor->prev = insertNode;
        L->length++;
        L->index++;
    }
    else
    {
        Node prevNode = L->cursor->prev;
        insertNode->prev = prevNode;
        prevNode->next = L->cursor;
        L->cursor->prev = insertNode;
        L->length++;
        L->index++;
    }
}
void insertAfter(List L, void *data)
{
    Node insertNode = newNode(data);
    if (L->length <= 0)
    {
        printf("List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->index == -1)
    {
        printf("List Error: calling insertAfter() on undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    if (L->index == 0 && L->length == 1)
    {
        Node prevNode = L->front;
        L->back = insertNode;
        insertNode->prev = prevNode;
        prevNode->next = insertNode;
        L->length++;
        return;
    }
    if (L->index == 0)
    {
        Node afterNode = L->cursor->next;
        L->cursor->next = insertNode;
        insertNode->prev = L->cursor;
        afterNode->prev = insertNode;
        insertNode->next = afterNode;
        L->length++;
        return;
    }
    if (L->index == (L->length - 1))
    {
        L->cursor->next = insertNode;
        insertNode->prev = L->cursor;
        L->back = insertNode;
        L->length++;
        return;
    }

    Node afterNode = L->cursor->next;
    L->cursor->next = insertNode;
    insertNode->prev = L->cursor;
    insertNode->next = afterNode;
    afterNode->prev = insertNode;
    L->length++;
}
void deleteFront(List L)
{
    Node N = NULL;

    if (L == NULL)
    {
        printf("List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L))
    {
        printf("List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    N = L->front;
    if (length(L) > 1)
    {
        L->front = L->front->next;
    }
    else
    {
        L->front = L->back = NULL;
    }
    L->length--;
    if (index(L) != -1)
    {
        L->index--;
    }
    freeNode(&N);
}
void deleteBack(List L)
{
    if (L == NULL)
    {
        printf("List Error: Calling deleteBack() on NULL List reference\n");
        exit(1);
    }
    Node N = NULL;
    if (L != NULL)
    {
        if (L->cursor == L->back)
        {
            L->cursor = NULL;
            L->index = -1;
        }
        if (L->length > 1)
        {
            N = L->back;
            L->back = L->back->prev;
            L->back->next = NULL;
            freeNode(&N);
        }
        else
        {
            N = L->back;
            freeNode(&N);
            L->back = NULL;
        }
        L->length--;
    }
}
void delete(List L)
{
    if (L->index == (L->length - 1))
    {
        deleteBack(L);
        return;
    }
    else if (L->index == 0)
    {
        deleteFront(L);
        return;
    }
    Node prevNode = L->cursor->prev;
    Node nextNode = L->cursor->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
    L->index = -1;
    L->length--;
    freeNode(&L->cursor);
}
// Other operations -----------------------------------------------------------
void printList(FILE *out, List L)
{
    Node iterNode = L->front;
    for (int i = 0; i < L->length; i++)
    {
        if (iterNode != NULL)
        {
            Node printNode = iterNode;
            iterNode = iterNode->next;
            fprintf(out, "%p", printNode->data);
            if (iterNode != NULL)
            {
                fprintf(out, " ");
            }
        }
    }
}

// Optional operations --------------------------------------------------------
List concatList(List A, List B); // Returns a new List which is the concatenation of
                                 // A and B. The cursor in the new List is undefined,
                                 // regardless of the states of the cursors in A and B.
                                 // The states of A and B are unchanged.

// Helper operations ----------------------------------------------------------
bool isEmpty(List L)
{
    if (L == NULL)
    {
        printf("List Error: calling empty() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    return (L->length == 0);
}
