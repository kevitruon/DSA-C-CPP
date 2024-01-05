/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Sparse.c
 ************************************************************/
#include <stdio.h>
#include "Matrix.h"

int main(int argc, char *argv[])
{
    int n, i, nnza, nnzb, row, col;
    double val;
    FILE *in;
    FILE *out;
    Matrix A, B, C, D, E, F, G, H, I, J;

    // check command line for correct number of arguments
    if (argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // open input file for reading
    in = fopen(argv[1], "r");
    if (!in)
    {
        printf("Unable to read from file %s\n", argv[1]);
        exit(1);
    }

    // open output file for writing
    out = fopen(argv[2], "w");
    if (!out)
    {
        printf("Unable to write to file %s\n", argv[2]);
        exit(1);
    }

    // scan and initialize Matrix
    fscanf(in, " %d %d %d", &n, &nnza, &nnzb);

    // create two matrices A and B of size n
    A = newMatrix(n);
    B = newMatrix(n);

    // from 0 to nnza:
    //      scan in the rows, columns and values
    //      assign them to matrix A
    for (i = 0; i < nnza; i++)
    {
        fscanf(in, "%d %d %lf\n", &row, &col, &val);
        changeEntry(A, row, col, val);
    }
    // from 0 to nnzb:
    //      scan in the rows, columns and values
    //      assign them to matrix B
    for (i = 0; i < nnzb; i++)
    {
        fscanf(in, "%d %d %lf\n", &row, &col, &val);
        changeEntry(B, row, col, val);
    }

    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    // print out the matrix
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "\nB has %d non-zero entries:\n", NNZ(B));
    // print out the matrix
    printMatrix(out, B);
    fprintf(out, "\n");

    fprintf(out, "\n(1.5)*A =\n");
    C = scalarMult(1.5, A);
    // C = multiply everything in A by 1.5
    // print out the matrix
    printMatrix(out, C);
    fprintf(out, "\n");

    fprintf(out, "\nA+B =\n");
    // D = sum of a and b
    //  print out the matrix
    D = sum(A, B);
    printMatrix(out, D);
    fprintf(out, "\n");

    fprintf(out, "\nA+A =\n");
    // E = addition of A twice
    // print out the matrix
    E = sum(A, A);
    printMatrix(out, E);
    fprintf(out, "\n");

    fprintf(out, "\nB-A =\n");
    // F = subtraction of A from B;
    // print out the matrix
    F = diff(B, A);
    printMatrix(out, F);
    fprintf(out, "\n");

    fprintf(out, "\nA-A =\n");
    // G = subtraction of A from A (should be 0)
    // print out the matrix
    G = diff(A, A);
    printMatrix(out, G);
    fprintf(out, "\n");

    fprintf(out, "\nTranspose(A) =\n");
    // H = transpose of A
    // print out the matrix
    H = transpose(A);
    printMatrix(out, H);
    fprintf(out, "\n");

    fprintf(out, "\nA*B =\n");
    // I = product of the two matrices
    // print out the matrix
    I = product(A, B);
    printMatrix(out, I);
    fprintf(out, "\n");

    fprintf(out, "\nB*B =\n");
    // J = product of B twice
    // print out the matrix
    J = product(B, B);
    printMatrix(out, J);
    fprintf(out, "\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);
    fclose(in);
    fclose(out);
    // free all metrices
    // close files

    return 0;
}