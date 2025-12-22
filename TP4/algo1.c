#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include <time.h>

// structure personnalisee pour le temps haute précision sous windows
typedef struct {
    long tv_sec;
    long tv_nsec;
} high_res_time_t;

// fonctions de timing haute précision pour windows
void get_high_precision_time(high_res_time_t *ts) {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    ts->tv_sec = (long)(counter.QuadPart / frequency.QuadPart);
    ts->tv_nsec = (long)(((counter.QuadPart % frequency.QuadPart) * 1000000000LL) / frequency.QuadPart);
}

double get_time_diff_s(high_res_time_t *start, high_res_time_t *end) {
    return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}

double TriBulle(int T[], int n) {
    bool changement = true;
    int temp;

    high_res_time_t start, end;
    get_high_precision_time(&start);

    while (changement) {
        changement = false;

        for (int i = 0; i < n - 1; i++) {
            if (T[i] > T[i + 1]) {
                temp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = temp;
                changement = true;
            }
        }
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

double TriBulleOpt(int T[], int n) {
    bool changement = true;
    int temp;
    int m = n - 1;

    high_res_time_t start, end;
    get_high_precision_time(&start);

    while (changement) {
        changement = false;

        for (int i = 0; i < m; i++) {
            if (T[i] > T[i + 1]) {
                temp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = temp;
                changement = true;
            }
        }
        m--;
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

void remplir_aleatoire(int T[], int n) {
    for (int i = 0; i < n; i++) {
        T[i] = rand() % 100000;
    }
}

void copier_tableau(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int tailles[] = {100, 200, 300, 400, 500};
    int nbTailles = sizeof(tailles) / sizeof(tailles[0]);

    FILE *f = fopen("Algo1.csv", "w");
    if (!f) {
        printf("Erreur ouverture fichier CSV\n");
        return 1;
    }

    fprintf(f, "num_algo,n,temps_moyen\n");

    printf("Debut des tests\n\n");

    for (int t = 0; t < nbTailles; t++) {
        int n = tailles[t];

        int *T = malloc(n * sizeof(int));
        int *Tcopy = malloc(n * sizeof(int));

        double somme1 = 0.0;
        double somme2 = 0.0;

        for (int k = 0; k < 10; k++) {
            remplir_aleatoire(T, n);
            copier_tableau(T, Tcopy, n);
            somme1 += TriBulle(Tcopy, n);
        }

        for (int k = 0; k < 10; k++) {
            remplir_aleatoire(T, n);
            copier_tableau(T, Tcopy, n);
            somme2 += TriBulleOpt(Tcopy, n);
        }

        double moy1 = somme1 / 10.0;
        double moy2 = somme2 / 10.0;

        printf("n = %d\n", n);
        printf("  TriBulle    : %f s\n", moy1);
        printf("  TriBulleOpt : %f s\n\n", moy2);

        fprintf(f, "1,%d,%f\n", n, moy1);
        fprintf(f, "2,%d,%f\n", n, moy2);

        free(T);
        free(Tcopy);
    }

    fclose(f);

    printf("Resultats sauvegardes dans Algo1.csv\n");

    return 0;
}
