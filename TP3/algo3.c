#include <stdio.h>
#include <stdbool.h>

int sousMat2(int n, int m, int n2, int m2, int A[n][m], int B[n2][m2]) {
    for (int i = 0; i <= n - n2; i++) {
        for (int j = 0; j <= m - m2; j++) {
            if (A[i][j] > B[0][0] || A[i][j + m2 - 1] < B[0][0]) 
                continue;

            int found = 1;
            for (int x = 0; x < n2 && found; x++) {
                for (int y = 0; y < m2; y++) {
                    if (A[i + x][j + y] != B[x][y]) {
                        found = 0;
                        break;
                    }
                }
            }
            if (found) {
                return 1; 
            }
        }
    }
    return 0;
}

int main() {

    // Matrice A de base (avec répétitions)
    int A[4][5] = {
        {1, 1, 2, 3, 4},
        {2, 3, 3, 4, 5},
        {3, 4, 4, 5, 6},
        {4, 5, 5, 6, 7}
    };

    // 1️⃣ B existe au début d’une ligne
    int B1[2][2] = {{1, 1}, {2, 3}};

    // 2️⃣ B existe au milieu d’une ligne
    int B2[2][2] = {{2, 3}, {3, 4}};

    // 3️⃣ B existe à la fin d’une ligne
    int B3[2][2] = {{3, 4}, {4, 5}};

    // 4️⃣ B = 1 seule ligne, répétitions
    int B4[1][3] = {{1, 1, 2}};

    // 5️⃣ B = 1 seule colonne
    int B5[3][1] = {{2}, {3}, {4}};

    // 6️⃣ B n’existe pas (valeurs absentes)
    int B6[2][2] = {{5, 5}, {6, 6}};

    // 7️⃣ B plus grande que A (impossible)
    int B7[5][2] = {{1,2},{2,3},{3,4},{4,5},{5,6}};

    // 8️⃣ B = toute A (doit marcher)
    int B8[4][5] = {
        {1,1,2,3,4},
        {2,3,3,4,5},
        {3,4,4,5,6},
        {4,5,5,6,7}
    };

    // 9️⃣ B répétitions internes mais existe
    int B9[2][3] = {{3,3,4}, {4,4,5}};

        int B10[2][2] = {{3,4}, {4,5}};
        int B11[2][2] = {{3,3}, {4,4}};
        int B12[2][2] = {{4,5}, {5,6}};
 
    int B13[4][6] = {
        {1,1,2,3,4,3},
        {2,3,3,4,5,4},
        {3,4,4,5,6,5},
        {4,5,5,6,7,6}
    };


    printf("B1 : %d (attendu 1)\n", sousMat2(4,5,2,2,A,B1));
    printf("B2 : %d (attendu 1)\n", sousMat2(4,5,2,2,A,B2));
    printf("B3 : %d (attendu 1)\n", sousMat2(4,5,2,2,A,B3));
    printf("B4 : %d (attendu 1)\n", sousMat2(4,5,1,3,A,B4));
    printf("B5 : %d (attendu 1)\n", sousMat2(4,5,3,1,A,B5));
    printf("B6 : %d (attendu 0)\n", sousMat2(4,5,2,2,A,B6));
    printf("B7 : %d (attendu 0)\n", sousMat2(4,5,5,2,A,B7));
    printf("B8 : %d (attendu 1)\n", sousMat2(4,5,4,5,A,B8));
    printf("B9 : %d (attendu 1)\n", sousMat2(4,5,2,3,A,B9));
    printf("B10 : %d (attendu 1)\n", sousMat2(4,5,2,2,A,B10));
    printf("B11 : %d (attendu 1)\n", sousMat2(4,5,2,2,A,B11));
    printf("B12 : %d (attendu 1)\n", sousMat2(4,5,2,2,A,B12));
    printf("B13 : %d (attendu 0)\n", sousMat2(4,5,4,6,A,B13));



    return 0;
}
