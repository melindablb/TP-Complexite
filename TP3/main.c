//
// Created by HP on 15.12.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>

/*********************************** FONCTIONS TEMPS ***********************************/

typedef struct {
    long tv_sec;
    long tv_nsec;
} high_res_time_t;

void get_high_precision_time(high_res_time_t *ts) {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    ts->tv_sec  = (long)(counter.QuadPart / frequency.QuadPart);
    ts->tv_nsec = (long)(((counter.QuadPart % frequency.QuadPart) * 1000000000LL) / frequency.QuadPart);
}

double get_time_diff_s(high_res_time_t *start, high_res_time_t *end) {
    return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}

/*********************************** FONCTIONS DE CREATION DE MATRICES ***********************************/

int **allouer_matrice(int lignes, int cols) {
    // Allocation d'un tableau de pointeurs pour les lignes
    int **matrice = (int **)malloc(lignes * sizeof(int *));
    if (matrice == NULL) {
           fprintf(stderr, "Echec de l'allocation memoire pour les lignes de la matrice.\n");
        return NULL;
    }
    // Pour chaque ligne, allocation d'un tableau pour les colonnes
    for (int i = 0; i < lignes; i++) {
        matrice[i] = (int *)malloc(cols * sizeof(int));
        if (matrice[i] == NULL) {
            fprintf(stderr, "Echec de l'allocation memoire pour les colonnes de la matrice.\n");
            // En cas d'echec, liberer toute la memoire deje allouee
            for (int j = 0; j < i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            return NULL;
        }
    }
    return matrice;
}

// Remplit une matrice avec des nombres aleatoires entre 0 et 99.
void remplir_matrice(int n, int m, int **matrice) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrice[i][j] = rand() % 100;
        }
    }
}

void remplir_matriceTrie(int n, int m, int **matrice) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        //1er elm de la ligne
        matrice[i][0] = rand() % 100;
        for (int j = 1; j < m; j++) {
            matrice[i][j] = matrice[i][j-1]+rand() % 100;
        }
    }
}

// Libere la memoire allouee dynamiquement pour une matrice.
void liberer_matrice(int lignes, int **matrice) {
    if (matrice == NULL) return;
    // Libere d'abord chaque ligne
    for (int i = 0; i < lignes; i++) {
        free(matrice[i]);
    }
    // Libere ensuite le tableau de pointeurs
    free(matrice);
}


/*********************************** FONCTIONS DEMANDEES ***********************************/

double produitMatrices(int n, int m, int p, int **A, int **B, int **C) {
    high_res_time_t start, end;
    get_high_precision_time(&start);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < m; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

int sousMat1 (int n, int m, int** A, int n2, int m2, int **B, int *ligne, int *col, double *temps) {
    high_res_time_t start, end;
    get_high_precision_time(&start);
    for (int i = 0; i <= n - n2; i++) {
        for (int j = 0; j <= m - m2; j++) {
            int match = 1;
            for (int k = 0; k < n2; k++) {
                for (int l = 0; l < m2; l++) {
                    if (A[i + k][j + l] != B[k][l]) {
                        match = 0;
                        break;
                    }
                }
                if (!match) {
                    break;
                }
            }
            if (match) {
                *ligne = i;
                *col = j;
                get_high_precision_time(&end);
                *temps=get_time_diff_s(&start, &end);
                return 1;
            }
        }
    }
    get_high_precision_time(&end);
    *temps=get_time_diff_s(&start, &end);
    return 0;
}

int sousMat2(int n, int m, int n2, int m2, int** A, int **B,int *ligne, int *col,double *temps) {
    high_res_time_t start, end;
    get_high_precision_time(&start);

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
                *ligne = i;
                *col = j;
                get_high_precision_time(&end);
                *temps=get_time_diff_s(&start, &end);
                return 1;
            }
        }
    }
    get_high_precision_time(&end);
    *temps=get_time_diff_s(&start, &end);
    return 0;
}

/*********************************** MAIN: GENERATION DES CSV ***********************************/


int main() {

    /************************** PARTIE 1 **************************/
    int **B; // m=60 p=70
    B=allouer_matrice(60, 70);
    if (B == NULL) {
        printf("Erreur d'allocation matrice B\n");
        return 1;
    }

    remplir_matrice(60, 70, B);
    int valeurs_n[6]={100,500,1000,2000,5000,10000};
    int i=0,n;

    //fichier csv
    FILE *f=fopen("Partie1.csv","w");
    fprintf(f,"n,temps\n");

    while (i<6) {
        double time;
        n=valeurs_n[i];

        int **A;
        A=allouer_matrice(n, 60);

        if (A == NULL) {
            printf("Erreur d'allocation matrice A\n");
            return 1;
        }
        remplir_matrice(n, 60, A);

        int **C;
        C=allouer_matrice(n,70);

        if (C == NULL) {
            printf("Erreur d'allocation matrice C\n");
            return 1;
        }

        //fonction produit
        time=produitMatrices(n,60,70,A,B,C);

        //remplissage du csv
        fprintf(f,"%d,%.9f\n",n,time);
        liberer_matrice(n,A);
        liberer_matrice(n,C);
        i++;
    }

    liberer_matrice(60, B);
    fclose(f);



    /************************** PARTIE 2 **************************/
    //1- en faisant varier n et m

    FILE *f2=fopen("Partie2-1.csv","w");
    fprintf(f2,"algo,n1,m1,temps\n");

    int valeurs_n1[6]={100,500,1000,2000,5000,10000};
    int n1;
    i=0;
    int **B2;
    B2=allouer_matrice(10, 10);
    if (B2 == NULL) {
        printf("Erreur d'allocation matrice B\n");
        return 1;
    }
    remplir_matriceTrie(10,10,B2); //remplissage de B

    while (i<6) {
        double time;
        int l,c;
        n1=valeurs_n1[i];
        int **A2;
        A2=allouer_matrice(n1,n1);
        if (A2 == NULL) {
            printf("Erreur d'allocation matrice A\n");
            return 1;
        }
        remplir_matriceTrie(n1,n1,A2); //remplissage de A

        sousMat1(n1,n1,A2,10,10,B2,&l,&c,&time);
        fprintf(f2,"1,%d,%d,%.9f\n",n1,n1,time);

        sousMat2(n1,n1,10,10,A2,B2,&l,&c,&time);
        fprintf(f2,"2,%d,%d,%.9f\n",n1,n1,time);

        liberer_matrice(n1,A2);
        i++;
    }
    liberer_matrice(10,B2);
    fclose(f2);

    //2- en faisant varier n' et m'

    FILE *f3=fopen("Partie2-2.csv","w");
    fprintf(f3,"algo,n2,m2,temps\n");

    int valeurs_m1[6]={100,500,1000,2000,5000,9000};
    int m1;
    i=0;
    int **A3;
    A3=allouer_matrice(10000,10000);
    if (A3 == NULL) {
        printf("Erreur d'allocation matrice A\n");
        return 1;
    }
    remplir_matriceTrie(10000,10000,A3); //remplissage de A

    while (i<6) {
        double time;
        int l,c;
        m1=valeurs_m1[i];
        int **B3;
        B3=allouer_matrice(m1,m1);
        if (B3 == NULL) {
            printf("Erreur d'allocation matrice B\n");
            return 1;
        }
        remplir_matriceTrie(m1,m1,B3); //remplissage de B

        sousMat1(10000,10000,A3,m1,m1,B3,&l,&c,&time);
        fprintf(f3,"1,%d,%d,%.9f\n",m1,m1,time);

        sousMat2(10000,10000,m1,m1,A3,B3,&l,&c,&time);
        fprintf(f3,"2,%d,%d,%.9f\n",m1,m1,time);

        liberer_matrice(m1,B3);
        i++;
    }

    liberer_matrice(10000,A3);
    fclose(f3);

    printf("Fichiers csv crees avec succes.\n");

    return 0;
}
