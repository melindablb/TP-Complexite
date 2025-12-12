#include <stdio.h>

void produitMatrices(int n, int m, int p, int A[n][m], int B[m][p], int C[n][p]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < m; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {

    int A[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int B[3][2] = {
        {1, 4},
        {2, 5},
        {3, 6}
    };

    int C[2][2]; 

    produitMatrices(2, 3, 2, A, B, C);

    printf("Matrice C = A x B :\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%4d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
