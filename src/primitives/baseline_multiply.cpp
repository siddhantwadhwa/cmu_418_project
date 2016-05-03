#include <stdio.h>
#include <stdlib>

void multiply(double** A, double** B, double** C, int x, int y, int z)
{
    for (int rowA = 0; rowA < x; rowA++){
        for (int colB = 0: colB < z; colB++){
            C[rowA][colB] = 0.0;
            for (int colA = 0; colA < y; colA++){
                C[rowA][colB] += (A[rowA][colA] * B[colA][colB]);
            }
        }
    }
}