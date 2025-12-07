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

void genereTableauTrieAvecRepetition(long long tab[], long long n) { //genere tableau trie avec repetitions
    tab[0] = (long long)rand() % 1000L;

    for (long long i = 1; i < n; i++) {
        tab[i] = tab[i-1] + ((long long)rand() % 5L);
    }
}

double rechElets_Dicho(long long tab[], long long n, long long x,int* trouve){

    int deb = 0;
    int fin = n-1;
    int m;

    double time_taken;
    high_res_time_t start, end;
    get_high_precision_time(&start);

    while (deb <= fin){
        m = (deb + fin) / 2;

        if (tab[m] == x) {
            *trouve=1;
            get_high_precision_time(&end);
            time_taken = get_time_diff_s(&start, &end);
            return time_taken;
        }
        else if (tab[m] < x) {
            deb = m + 1;
        }
        else {
            fin = m - 1;
        }
    }
    *trouve=0;

    get_high_precision_time(&end);
    time_taken = get_time_diff_s(&start, &end);
    return time_taken;
    }


int main() {
    double temps1,temps2;
    srand(time(NULL));
    long long valeurs_n[10] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000};
    int i=0,trouve;

    FILE *f = fopen("dicho.csv", "w");
    fprintf(f, "Algo,N,BestCase,WorstCase\n");

    while (i<10) {
        printf("\n\nTaille N: %lld ",valeurs_n[i]);
        long long *tab = malloc(valeurs_n[i] * sizeof(long long));
        if (!tab) return 1;

        genereTableauTrieAvecRepetition(tab,valeurs_n[i]);

        //pire cas
        temps1=rechElets_Dicho(tab,valeurs_n[i],(tab[(valeurs_n[i])-1]+1000),&trouve);
        printf("\nPire cas x=%lld, trouve:%d, temps: %.9f\n",tab[(valeurs_n[i])-1]+1,trouve,temps1);

        //meilleur cas
        temps2=rechElets_Dicho(tab,valeurs_n[i],tab[((valeurs_n[i])-1)/2],&trouve);
        printf("Meilleur cas x=%lld, trouve:%d, temps: %.9f",tab[((valeurs_n[i])-1)/2],trouve,temps2);

        fprintf(f,"3,%lld,%.9f,%.9f\n",valeurs_n[i],temps1,temps2);
        i++;
        free(tab);
    }

    fclose(f);
    return 0;
}