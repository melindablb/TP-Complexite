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
    ts->tv_nsec = (long)(((counter.QuadPart % frequency.QuadPart) * 1000000000LL)
                          / frequency.QuadPart);
}

double get_time_diff_s(high_res_time_t *start, high_res_time_t *end) {
    return (end->tv_sec - start->tv_sec)
         + (end->tv_nsec - start->tv_nsec) / 1e9;
}


void genererTabAleatoire(int tab[], int n) {
    for (int i = 0; i < n; i++)
        tab[i] = rand() % 100000;
}

void copierTab(int src[], int dst[], int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/*Tri rapide*/
int partitionner(int tab[], int d, int f) {
    int pivot = tab[d];
    int i = d + 1;
    int j = f;
    int temp;
    while (1) {
        while (i <= f && tab[i] <= pivot) i++;
        while (j >= d && tab[j] > pivot) j--;
        if (i < j) {
            temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
        } else break;
    }
    temp = tab[d];
    tab[d] = tab[j];
    tab[j] = temp;
    return j;
}

double triRapide(int tab[], int n) {
    high_res_time_t start, end;
    get_high_precision_time(&start);


    void triRapideRec(int tab[], int p, int r) {
        if (p < r) {
            int q = partitionner(tab, p, r);
            triRapideRec(tab, p, q - 1);
            triRapideRec(tab, q + 1, r);
        }
    }
    triRapideRec(tab, 0, n - 1);

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

/*  TRI PAR TAS */
void entasser(int tab[], int taille, int i) {
    int plusGrand = i;
    int gauche = 2*i + 1;
    int droite = 2*i + 2;

    if (gauche < taille && tab[gauche] > tab[plusGrand])
        plusGrand = gauche;
    if (droite < taille && tab[droite] > tab[plusGrand])
        plusGrand = droite;
    if (plusGrand != i) {
        int temp = tab[i];
        tab[i] = tab[plusGrand];
        tab[plusGrand] = temp;
        entasser(tab, taille, plusGrand);
    }
}

double triParTas(int tab[], int n) {
    high_res_time_t start, end;
    get_high_precision_time(&start);

    // construire le tas
    for (int i = n / 2 - 1; i >= 0; i--)
        entasser(tab, n, i);

    // extraire un par un
    for (int i = n - 1; i >= 0; i--) {
        int temp = tab[0];
        tab[0] = tab[i];
        tab[i] = temp;
        entasser(tab, i, 0);
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}


int main() {
    srand((unsigned)time(NULL));

    int tailles[] = {200000, 400000, 600000, 800000,700000, 1000000, 1100000};
    int nbTailles = sizeof(tailles) / sizeof(tailles[0]);

    FILE *fRapide = fopen("triRapide_temps.csv", "w");
    fprintf(fRapide, "n,temps_moyen\n");

    FILE *fTas = fopen("triTas_temps.csv", "w");
    fprintf(fTas, "n,temps_moyen\n");

    for (int t = 0; t < nbTailles; t++) {
        int n = tailles[t];

        int *tabBase = malloc(n * sizeof(int));
        int *tabTemp = malloc(n * sizeof(int));

        double sommeTempsRapide = 0.0;
        double sommeTempsTas    = 0.0;

        for (int rep = 0; rep < 10; rep++) {
            genererTabAleatoire(tabBase, n);

            copierTab(tabBase, tabTemp, n);
            sommeTempsRapide += triRapide(tabTemp, n);

            copierTab(tabBase, tabTemp, n);
            sommeTempsTas += triParTas(tabTemp, n);
        }

        double moyenneRapide = sommeTempsRapide / 10.0;
        double moyenneTas    = sommeTempsTas / 10.0;

        printf("n=%d : triRapide=%.9f s, triParTas=%.9f s\n",
               n, moyenneRapide, moyenneTas);

        fprintf(fRapide, "%d,%.9f\n", n, moyenneRapide);
        fprintf(fTas,    "%d,%.9f\n", n, moyenneTas);

        free(tabBase);
        free(tabTemp);
    }

    fclose(fRapide);
    fclose(fTas);

    printf("CSV crees: triRapide_temps.csv et triTas_temps.csv\n");
    return 0;
}

