#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <intrin.h>
#include <stdint.h>
#include <windows.h>
#include <stdbool.h>

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


void genereTableauSansRepetition(long long tab[], long long n) {
    if (n <= 0) return;
    int bit;
    for (long long i = 0; i < n; i++) {
        tab[i] = i;
    }
    for (int i = n - 1; i > 0; i--) {
        long long  j = (long)rand() % (i + 1);
        long long temp = tab[i];
        if ((bit = (rand() > RAND_MAX / 2))==1) {
            tab[i] = -1*tab[j];
        }
        tab[j] = temp;
    }
}

double MaxEtMinA(long long tab[], long long n, long long * Max, long long * Min, long long * CompteurA) { //retourne le temps
    double time_taken;
    high_res_time_t start, end;
    *CompteurA=0;
    get_high_precision_time(&start);

    *Max = tab[0];
    *Min = tab[0];
    for (int i = 1; i < n; i++) {
        (*CompteurA) ++;
        if (*Min > tab[i]) {
            *Min = tab[i];
        }
        (*CompteurA) ++;
        if (*Max < tab[i]) {
            *Max = tab[i];
        }
    }

    get_high_precision_time(&end);
    time_taken = get_time_diff_s(&start, &end);

    return time_taken;
}

double MaxEtMinB(long long tab[], long long n, long long *Max, long long *Min, long long *CompteurB) {
    double time_taken;
    high_res_time_t start, end;
    *CompteurB = 0;

    get_high_precision_time(&start);
    //si la taille est paire on compare les deux premiers pour init le max et min sinon on prend que le 1er elmnt
    if (n % 2 == 0) {
        *Max = tab[0] > tab[1] ? tab[0] : tab[1];
        *Min = tab[0] < tab[1] ? tab[0] : tab[1];
    } else {
        *Max = *Min = tab[0];
    }

    for (int i = 0; i < n-1; i += 2) {
        (*CompteurB)++;
        int a = tab[i];
        int b = tab[i+1];
        if (a > b) {
            tab[i] = b;
            tab[i+1] = a;
        } else {
            tab[i] = a;
            tab[i+1] = b;
        }

        (*CompteurB)++;
        if (tab[i] < *Min) *Min = tab[i];
        (*CompteurB)++;
        if (tab[i+1] > *Max) *Max = tab[i+1];
    }

    if (n % 2 != 0) {
        if (tab[n-1] < *Min) *Min = tab[n-1];
        if (tab[n-1] > *Max) *Max = tab[n-1];
    }
    get_high_precision_time(&end);
    time_taken = get_time_diff_s(&start, &end);
    return time_taken;
}


int main() {
    FILE *f = fopen("PARTIE2.csv", "w");
    fprintf(f, "Algo,N,Temps,Compteur\n");

    double temps1,temps2;
    int t,i=0;
    long long valeurs_n[10] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000};

    while (i<10) {
        long long *tab = malloc(valeurs_n[i] * sizeof(long long));
        if (!tab) return 1;

        genereTableauSansRepetition(tab, valeurs_n[i]);

        long long Max,Min,Max2,Min2;
        long long CompteurA,CompteurB;

        temps1=MaxEtMinA(tab,valeurs_n[i],&Max,&Min,&CompteurA);
        fprintf(f,"A,%lld,%f,%lld\n",valeurs_n[i],temps1,CompteurA);
        temps2=MaxEtMinB(tab,valeurs_n[i],&Max2,&Min2,&CompteurB);
        fprintf(f,"B,%lld,%f,%lld\n",valeurs_n[i],temps2,CompteurB);

        printf("\nTaille du tableau: %lld, Max: %lld, Min: %lld\nTemps MaxEtMinA: %f, Nombres de comparaisons: %lld\nTemps MaxEtMinB: %f, Nombres de comparaisons: %lld\n",valeurs_n[i], Max,Min,temps1,CompteurA,temps2,CompteurB);
        free(tab);
        i++;
    }
    fclose(f);
    return 0;


}