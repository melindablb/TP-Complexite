#include <stdio.h>


int sousMat1(int n, int m, int n2, int m2, int A[n][m], int B[n2][m2]) {

    for (int i = 0; i <= n - n2; i++) {
        for (int j = 0; j <= m - m2; j++) {

            int ok = 1;
            for (int x = 0; x < n2 && ok; x++) {
                for (int y = 0; y < m2 && ok; y++) {
                    if (A[i + x][j + y] != B[x][y]) {
                        ok = 0;
                    }
                }
            }

            if (ok) return 1;
        }
    }

    return 0;
}

int main() {

    int A[4][5] = {
        {1, 2, 3, 4, 5},
        {5, 6, 7, 8, 9},
        {9, 1, 2, 3, 4},
        {4, 5, 6, 7, 8}
    };


    int B[2][2] = {
        {-1, 7},
        {1, 2}
    };

    int res = sousMat1(4, 5, 2, 2, A, B);

    if (res == 1)
        printf("La matrice B est une sous-matrice de A.\n");
    else
        printf("La matrice B n'est PAS une sous-matrice de A.\n");

    return 0;
}
