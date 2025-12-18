//
// Created by HP on 15.12.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>

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

void genererMat(int n, int m, int A[n][m]) { //matrice quelconque
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = rand() % 1000;
        }
    }
}

void genererMatTrie(int n, int m, int A[n][m]) { //matrice trie par ligne
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        //1er elm de la ligne
        A[i][0] = rand() % 1000;
        for (int j = 1; j < m; j++) {
            A[i][j] = A[i][j-1]+rand() % 1000;
        }
    }
}

double produitMatrices(int n, int m, int p, int A[n][m], int B[m][p], int C[n][p]) {
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

int sousMat1 (int n, int m, int A[n][m], int n2, int m2, int B[n2][m2], int *ligne, int *col, double *temps) {
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

int sousMat2(int n, int m, int n2, int m2, int A[n][m], int B[n2][m2],int *ligne, int *col,double *temps) {
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

int main() {

    /************************** PARTIE 1 **************************/
    int B[60][70]; // m=60 p=70
    genererMat(60, 70, B); // remplissage de B

    int valeurs_n[6]={10,50,100,250,500,1000};
    int i=0,n;

    //fichier csv
    FILE *f=fopen("Partie1.csv","w");
    fprintf(f,"n,temps\n");

    while (i<6) {
        double time;
        n=valeurs_n[i];

        int A[n][60];
        int C[n][70];
        //remplissage de A
        genererMat(n,60,A);

        //fonction produit
        time=produitMatrices(n,60,70,A,B,C);

        //remplissage du csv
        fprintf(f,"%d,%.9f\n",n,time);

        i++;
    }

    fclose(f);



    /************************** PARTIE 2 **************************/
    //1- en faisant varier n et m

    FILE *f2=fopen("Partie2-1.csv","w");
    fprintf(f2,"algo,n1,m1,temps\n");

    int valeurs_n1[6]={10,50,100,200,300,500};
    int n1;
    i=0;
    int B2[10][10];
    genererMatTrie(10,10,B2); //remplissage de B

    while (i<6) {
        double time;
        int l,c;
        n1=valeurs_n1[i];
        int A2[n1][n1];
        genererMatTrie(n1,n1,A2); //remplissage de A

        sousMat1(n1,n1,A2,10,10,B2,&l,&c,&time);

        fprintf(f2,"1,%d,%d,%.9f\n",n1,n1,time);

        sousMat2(n1,n1,10,10,A2,B2,&l,&c,&time);
        fprintf(f2,"2,%d,%d,%.9f\n",n1,n1,time);

        i++;
    }
    fclose(f2);

    //2- en faisant varier n' et m'

    FILE *f3=fopen("Partie2-2.csv","w");
    fprintf(f3,"algo,n2,m2,temps\n");

    int valeurs_m1[6]={10,50,100,200,300,500};
    int m1;
    i=0;
    int A3[70][70];
    genererMatTrie(70,70,A3); //remplissage de A

    while (i<6) {
        double time;
        int l,c;
        m1=valeurs_m1[i];
        int B3[m1][m1];
        genererMatTrie(m1,m1,B3); //remplissage de B

        sousMat1(70,70,A3,m1,m1,B3,&l,&c,&time);
        fprintf(f3,"1,%d,%d,%.9f\n",m1,m1,time);

        sousMat2(70,70,m1,m1,A3,B3,&l,&c,&time);
        fprintf(f3,"2,%d,%d,%.9f\n",m1,m1,time);

        i++;
    }

    fclose(f3);

    printf("Fichiers csv crees avec succes.\n");

    return 0;
}