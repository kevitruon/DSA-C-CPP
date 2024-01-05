/************************************************************
*   Name:   Kevin Truong (#1834510)
*   Email:  ktruon13@ucsc.edu
*   Class:  CSE101-02 (Tantalo) Winter 2023
*   File:   ListTest.c
************************************************************/
/****************************************************************************************
*  ListTest.c
*  Test for List ADT
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"

void testNewList() {
    List L = newList();
    assert(L != NULL);
    assert(isEmpty(L));
    freeList(&L);
}

void testInsert() {
    List L = newList();
    insertBefore(L, 10);
    assert(length(L) == 1);
    assert(front(L) == 10);
    assert(back(L) == 10);
    insertAfter(L, 20);
    insertBefore(L, 5);
    assert(front(L) == 5);
    assert(back(L) == 20);
    assert(length(L) == 3);
    freeList(&L);
}

void testDelete() {
    List L = newList();
    insertBefore(L, 10);
    insertAfter(L, 20);
    insertBefore(L, 5);
    assert(length(L) == 3);
    delete(L);
    assert(length(L) == 2);
    assert(front(L) == 5);
    assert(back(L) == 20);
    freeList(&L);
}

void testOther() {
    List L = newList();
    insertBefore(L, 10);
    insertAfter(L, 20);
    insertBefore(L, 5);
    moveFront(L);
    assert(get(L) == 5);
    moveNext(L);
    moveNext(L);
    movePrev(L);
    assert(get(L) == 20);
    assert(index(L) == 1);
    clear(L);
    assert(isEmpty(L));
    freeList(&L);
}

int main() {
    testNewList();
    testInsert();
    testDelete();
    testOther();
    printf("All test cases passed!\n");
    return 0;
}
