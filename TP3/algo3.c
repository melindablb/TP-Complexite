#include <stdio.h>

// recoit en prmt une ligne triée de taille' m et la valeur a rechercher
int rechercheBinaire(int ligne[], int m, int valeur) {
    int g = 0, d = m - 1;
    while (g <= d) {
        int mid = (g + d) / 2;
        if (ligne[mid] == valeur) return mid;
        else if (ligne[mid] < valeur) g = mid + 1;
        else d = mid - 1;
    }
    return -1;
}

int sousMat2(int n, int m, int n2, int m2, int A[n][m], int B[n2][m2]) {

    int first = B[0][0];

    for (int i = 0; i <= n - n2; i++) {

        int j = rechercheBinaire(A[i], m, first);
        if (j == -1) continue;

        if (j + m2 > m) continue; 

        int ok = 1;
        for (int x = 0; x < n2 && ok; x++) {
            for (int y = 0; y < m2 && ok; y++) {
                if (A[i + x][j + y] != B[x][y])
                    ok = 0;
            }
        }
        if (ok) return 1;
    }

    return 0;
}

int main() {
    int A[4][5] = {
        {1, 2, 3, 4, 5},
        {2, 3, 4, 5, 6},
        {3, 4, 5, 6, 7},
        {4, 5, 6, 7, 8}
    };

    int B[2][2] = {
        {6, 7},
        {7, 8}
    };
    

    if (sousMat2(4, 5, 2, 2, A, B))
        printf("B est dans A !\n");
    else
        printf("B n'est pas dans A !\n");

    return 0;
}
