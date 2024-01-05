/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Matrix.c
 ************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"

// Set "Entry" alais to EntryObj
typedef struct EntryObj *Entry;

// private Entry Object type
typedef struct EntryObj
{
    int col;
    double val;
} EntryObj;

typedef struct MatrixObj
{
    int size;
    int NNZ;
    List *row;
} MatrixObj;

// constructor for entry
Entry newEntry(int column, double value)
{
    if (column < 0)
    {
        printf("Matrix Error: calling newEntry() with invalid column reference\n");
        exit(1);
    }
    Entry E = malloc(sizeof(EntryObj));
    E->col = column;
    E->val = value;
    return E;
}

void deleteEntry(Entry *pE)
{
    if (pE != NULL && *pE != NULL)
    {
        free(*pE);
        *pE = NULL;
    }
}

// constructor for matrix
Matrix newMatrix(int n)
{
    if (n < 0)
    { // Base case, "n" can not be less than 0
        printf("Matrix Error: calling newMatrix() with invalid n value\n");
        exit(EXIT_FAILURE);
    }
    Matrix M = malloc(sizeof(MatrixObj)); // allocate a Matrix "M"
    M->row = calloc(n+1, sizeof(List)); // allocate memory for M->list
    M->size = 0;
    M->NNZ = 0;
    for (int i = 1; i <= n; i++)
    {                          // from 1 to "n" inclusive:
        M->row[i] = newList(); // create a new list for each index of the array
        M->size++;
    }
    return M; // return "M";
}

// destructor for Matrix
void freeMatrix(Matrix *pM)
{
    // You guys know how to implement these by now
    // could use makeZero then free
    Matrix M = *pM;
    for (int i = 1; i < (M)->size; i++)
    {
        freeList(&(M->row[i]));
    }
    free(M);
    *pM = NULL;
}

// Return the size of square Matrix M.
int size(Matrix M)
{
    // You guys know how to implement these by now
    if (M == NULL)
    {
        printf("Matrix Error: calling size() with NULL Matrix reference\n");
        exit(1);
    }
    return M->size;
}

// Return the number of non-zero elements in M.
int NNZ(Matrix M)
{
    // You guys know how to implement these by now
    if (M == NULL)
    {
        printf("Matrix Error: calling NNZ() with NULL Matrix reference\n");
        exit(1);
    }
    return M->NNZ;
}

// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B)
{
    if (A == NULL || B == NULL)
    {
        printf("Matrix Error: calling equals() with NULL Matrix reference\n");
        exit(1);
    }
    if (A->size != B->size)
    {
        return false;
    }
    // similar to List implementation of equalls
    // check if lengths of the metrics are equal and exit if they are not
    Entry x, y;
    List c, d;
    int i;

    for (i = 1; i <= size(A); i++)
    {                  // from "i" = 1 to size of "A":
        c = A->row[i]; // set "c" and "d" to the rows of "A" and "B"
        d = B->row[i];
        moveFront(c); // move to the front of both lists
        moveFront(d);
        while (index(c) >= 0 && index(d) >= 0)
        { // while the indexes of the lists[i] are GTE 0 && their sizes are equall:
            x = (Entry)get(A->row[i]);
            y = (Entry)get(B->row[i]);
            if (x->col != y->col || x->val != y->val)
            {
                return false; // if their values are not equal return false
            }
            moveNext(c); // else move next on "c" and "d"
            moveNext(d);
            if (index(c) != index(d))
            { // if indexs of c or d falls off the list (-1), return false
                return false;
            }
        }
    }
    return true;
}

// Re-sets M to the zero Matrix state.
void makeZero(Matrix M)
{
    if (M == NULL) // BASE CASE
    {
        printf("Matrix Error: calling makeZero() with NULL Matrix reference\n");
        exit(1);
    }
    M->NNZ = 0; // set NNZ to 0
    for (int i = 1; i <= size(M); i++)
    {
        moveFront(M->row[i]);
        while (index(M->row[i]) >= 0)
        {
            Entry pE = (Entry)get(M->row[i]);
            deleteEntry(&pE);
            moveNext(M->row[i]);
        }
        clear(M->row[i]);
    }

    // iterate through the matrix:
    //      for each row in the matrix:
    //          free the entries and move next on the rows
    //      clear each row
}

// Changes the ith row, jth col of M to the val x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x)
{
    if (M == NULL) // BASE CASE
    {
        printf("Matrix Error: calling changeEntry() with NULL Matrix reference\n");
        exit(1);
    }
    if (i < 1 && i > size(M))
    { // 2 Base cases
        printf("Matrix Error: calling changeEntry() with invalid row reference\n");
        exit(1);
    }
    if (j < 1 && j > size(M))
    {
        printf("Matrix Error: calling changeEntry() with invalid column reference\n");
        exit(1);
    }
    List L = M->row[i];
    Entry E = NULL;
    // Set the entry to null and assign List L to the row at index i

    for (moveFront(L); index(L) >= 0; moveNext(L))
    {
        E = (Entry)get(L);
        if (E->col == j)
        {
            break;
        }
    }

    if (isEmpty(L) || index(L) == -1)
    {
        if (x != 0)
        {
            append(L, newEntry(j, x));
            M->NNZ++;
            return;
        }
    }
    else if (E->col > j)
    {
        if (x != 0)
        {
            insertBefore(L, newEntry(j, x));
            M->NNZ++;
            return;
        }
    }
    else
    {
        if (x != 0)
        {
            E->val = x;
            return;
        }
        else
        {
            delete (L);
            M->NNZ--;
            return;
        }
    }

    //       iterate through the L until you reach column j, then break
    //       if L was empty of the index fell off the back of L
    //               append the new value x to L if x is not 0 (add to end of row)
    //               increment the NNZ
    //       else if still on L but past column j
    //               insert the new value x before the current index is x is not 0
    //               increment the NNZ
    //       else (still on L at column j)
    //               if x is not 0, set the entry value to x
    //               else delete L and decrement the NNZ
}
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A)
{
    if (A == NULL) // BASE CASE
    {
        printf("Matrix Error: calling copy() with NULL Matrix reference\n");
        exit(1);
    }
    Matrix copy = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;

    // iterate "i" from 1 to size of matrix "A" (inclusive):
    for (i = 1; i <= size(A); i++)
    {
        // move to the front of the matrix A's rows at index "i"
        moveFront(A->row[i]);
        // iterate through the rows[i] as long as the index is GTE 0:
        while (index(A->row[i]) >= 0)
        {
            // assign Entry E to the cursor's data
            E = (Entry)get(A->row[i]);
            col = E->col;                             // set col to the E's column
            val = E->val;                             // set val to the E's value
            append(copy->row[i], newEntry(col, val)); // append a new entry of (col, val) to matrix Copy's row at index "i"
            moveNext(A->row[i]);
        }
    }
    copy->NNZ = A->NNZ;
    return copy;
}

// Returns a reference to a new Matrix object representing the transpose of A.
Matrix transpose(Matrix A)
{
    if (A == NULL) // BASE CASE
    {
        printf("Matrix Error: calling transpose() with NULL Matrix reference\n");
        exit(1);
    }
    Matrix T = newMatrix(size(A));
    Entry E;
    int i;
    double val;

    // similar to Copy() but use "i" as the column now
    // iterate "i" from 1 to size of matrix "A" (inclusive):
    for (i = 1; i <= size(A); i++)
    {
        //      move to the front of the matrix A's rows at index "i"
        moveFront(A->row[i]);
        //      iterate through the rows[i] as long as the index is GTE 0:
        while (index(A->row[i]) >= 0)
        {
            E = (Entry)get(A->row[i]);                // assign Entry E to the cursor's data
                                                      // set col to the E's column
            val = E->val;                             // set val to the E's value
            append(T->row[E->col], newEntry(i, val)); // append a new entry of (i, val) to matrix Copy's row at index "i"
            moveNext(A->row[i]);
        }
    }
    T->NNZ = A->NNZ;
    return T;
}

// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A)
{
    if (A == NULL) // BASE CASE
    {
        printf("Matrix Error: calling scalarMult() with NULL Matrix reference\n");
        exit(1);
    }

    Matrix M = newMatrix(size(A));
    Entry E;
    int i, col;
    double val;
    // if x is 0 or M's nnz is 0 just return M
    if (x == 0 || A->NNZ == 0)
    {
        return M;
    }
    // set the matrix M's nnz to be the same as A
    // iterate "i" from 1 to size of matrix "A" (inclusive):
    for (i = 1; i <= size(A); i++)
    {
        moveFront(A->row[i]); // move to the front of the matrix A's rows at index "i"
        while (index(A->row[i]) >= 0)
        {
            E = (Entry)get(A->row[i]);
            col = E->col;
            val = E->val * x;
            append(M->row[i], newEntry(col, val));
            M->NNZ++;
            moveNext(A->row[i]);
        }
        //      iterate through the rows[i] as long as the index is GTE 0:
        //          assign Entry E to the cursor's data
        //          set col to the E's column
        //          set val to the E's value
        //          append a new entry of (i, val) to matrix Copy's row at index "i"
        //          increment the nnz value of copy and move to the next index of rows in A
    }
    return M;
}

// helper function vector sum for functions sum() diff()
// Assuming C is an empty List object, sets C to P+Q (if flag==1) or P-Q
// (if flag==-1), considered as sparse vectors.
// Pre: P!=Q, P and Q are rows in different Matrix objects.
void vecSum(List A, List B, List C, int sign)
{
    double x, y, z;
    Entry a, b;

    moveFront(A);
    moveFront(B);
    while (index(A) >= 0 && index(B) >= 0)
    {
        a = (Entry)get(A);
        b = (Entry)get(B);
        x = a->val;
        y = (b->val) * sign;
        if (a->col == b->col)
        {
            z = x + y;
            if (z != 0)
            {
                append(C, newEntry(a->col, z));
            }
            moveNext(A);
            moveNext(B);
        }
        else if (a->col < b->col)
        {
            append(C, newEntry(a->col, x));
            moveNext(A);
        }
        else
        {
            append(C, newEntry(b->col, y));
            moveNext(B);
        }
    }
    while (index(A) >= 0)
    {
        a = (Entry)get(A);
        append(C, newEntry(a->col, a->val));
        moveNext(A);
    }
    while (index(B) >= 0)
    {
        b = (Entry)get(B);
        append(C, newEntry(b->col, b->val * sign));
        moveNext(B);
    }
}

// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B)
{
    if (A == NULL || B == NULL) // BASE CASE
    {
        printf("Matrix Error: calling sum() with NULL Matrix reference\n");
        exit(1);
    }
    if (size(A) != size(B)) // BASE CASE
    {
        printf("Matrix Error: calling sum() with different size Matrices\n");
        exit(1);
    }

    Matrix Add = newMatrix(size(A));
    if (A == B)
    {
        Add = scalarMult(2, A);
        Add->NNZ = A->NNZ;
    }
    else
    {
        for (int i = 1; i <= size(A); i++)
        {
            vecSum(A->row[i], B->row[i], Add->row[i], 1);
            Add->NNZ += length(Add->row[i]);
        }
    }
    // if A and B are equall just set Add by calling scalarMult on either of the matrices by 2
    // else iterate "i" from 1 to size of the matrix:
    //      set ADD's row at index "i" by calling vecSum on A's rows[i], B's rows[i]

    return Add;
}

// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B)
{
    if (A == NULL || B == NULL) // BASE CASE
    {
        printf("Matrix Error: calling diff() with NULL Matrix reference\n");
        exit(1);
    }
    if (size(A) != size(B)) // BASE CASE
    {
        printf("Matrix Error: calling diff() with different size Matrices\n");
        exit(1);
    }

    Matrix Diff = newMatrix(size(A));
    Matrix Neg = scalarMult(-1, B);
    Diff = sum(A, Neg);
    freeMatrix(&Neg);
    return Diff;
}

// vectorDot()
// Returns the dot product of Lists P and Q considered as sparse vectors.
// Pre: P!=Q, P and Q are rows in different Matrix objects.
double vecDot(List A, List B)
{
    // Base Cases
    double dp = 0.0;
    Entry a, b;
    moveFront(A);
    moveFront(B);
    // move to the front of both lists
    // iterate through the two lists as long as their indexes are GTE 0:
    // assign Entry "a" and "b" to the cursor's data at list "A" and "B"
    while (index(A) >= 0 && index(B) >= 0)
    {
        a = (Entry)get(A);
        b = (Entry)get(B);
        if (a->col == b->col)
        {
            dp += a->val * b->val;
            moveNext(A);
            moveNext(B);
        }
        else if (a->col < b->col)
        {
            moveNext(A);
        }
        else
        {
            moveNext(B);
        }
    }
    // if the column of entry "a" equals "b's":
    //      increment dp by the multiplicaion of entry "a" and "b's" values
    //      move to the next element of List A and B
    // else if Entry "a's" column is LT "b's"
    //      catch up with list A by calling moveNext on it
    // else catch up with list B by calling moveNext on it

    return dp;
}

// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B)
{
    if (A == NULL || B == NULL) // BASE CASE
    {
        printf("Matrix Error: calling product() with NULL Matrix reference\n");
        exit(1);
    }
    if (size(A) != size(B)) // BASE CASE
    {
        printf("Matrix Error: calling product() with different size Matrices\n");
        exit(1);
    }
    Matrix M = newMatrix(size(A));
    Matrix T = transpose(B);

    int i, j;
    double x;
    for (i = 1; i <= size(A); i++)
    {
        for (j = 1; j <= size(A); j++)
        {
            x = vecDot(A->row[i], T->row[j]);
            if (x != 0)
            {
                append(M->row[i], newEntry(j, x));
                M->NNZ++;
            }
        }
    }
    // iterate "i" from 1 to size of Matrix A:
    //      iterate "j" from 1 to size of Matrix A:
    //          set "x" to the dot product of Matrix A's row[i] and T's row[j]
    //          if "x" isn't zero:
    //              append new entry (j, x) to Matrix M
    //              increment the nnz

    // free the transpose matrix
    freeMatrix(&T);
    return M;
}

// printMatrix()
void printMatrix(FILE *out, Matrix M)
{
    Entry E = newEntry(0, 0);
    int cont = 0;
    for (int i = 1; i <= size(M); i++)
    {
        cont = 0;
        moveFront((M->row[i]));
        if (index(M->row[i]) >= 0)
        {
            E = (Entry)get(M->row[i]);
            if (E)
            {
                fprintf(out, "%d: ", i);
                cont = 1;
            }
        }
        while (index((M->row[i])) >= 0)
        {
            E = (Entry)get(M->row[i]);
            if (E)
            {
                fprintf(out, "(%d, %.1f) ", E->col, E->val);
            }
            moveNext(M->row[i]);
        }
        if (cont == 1)
        {
            fprintf(out, "\n");
        }
    }
}
