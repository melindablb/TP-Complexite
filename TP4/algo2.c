#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>


// FONCTIONS TEMPS

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
//----------------------------------

// creation du tab
void remplir_aleatoire(int T[], int n) {
    for (int i = 0; i < n; i++) {
        T[i] = rand() % 100000;
    }
}


// fonction tri gnome

double gnome(int t[], int n)
{
    high_res_time_t start, end;
    get_high_precision_time(&start);

    int a;
    int i=0;

    while (i < n-1 )
    {
        if ( t[i] <= t[i+1] ) { i= i+1;}
        else{
            a= t[i];
            t[i]= t[i+1];
            t[i+1]=a;
            if (i==0){ i= i+1;}
            else { i= i-1; }
        }
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

int main() {
    srand(time(NULL));

    int size[] = {1000, 2000, 5000, 10000, 20000, 50000};
    int nbsize = sizeof(size) / sizeof(size[0]);

    FILE *f = fopen("Algo2.csv", "w");
    if (!f) {
        printf("Erreur ouverture fichier CSV\n");
        return 1;
    }

    fprintf(f, "num_algo,n,temps_moyen\n");

    printf("Debut des tests\n\n");

    int rep=10;

    for (int t = 0; t < nbsize; t++) {

        int n = size[t];

        // allocation
        int *tab = malloc(n * sizeof(int));
        if (!tab) {
            printf("Erreur malloc\n");
            return 1;
        }


        double somme_temps = 0.0;
        for (int r = 0; r < rep; r++) {
            remplir_aleatoire(tab, n);
            somme_temps += gnome(tab, n);
        }

        double temps_moyen = somme_temps / rep;

        // écriture CSV
        fprintf(f, "2,%d,%f\n", n, temps_moyen);

        printf("n=%d -> temps=%f s\n", n, temps_moyen);

        free(tab);
    }

    fclose(f);
    printf("Resultats sauvegardes .\n");

    return 0;
}