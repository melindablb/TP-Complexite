#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

// TEMPS HAUTE PRECISION

typedef struct {
    long tv_sec;
    long tv_nsec;
} high_res_time_t;

void get_high_precision_time(high_res_time_t *ts) {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    ts->tv_sec = (long)(counter.QuadPart / frequency.QuadPart);
    ts->tv_nsec = (long)(((counter.QuadPart % frequency.QuadPart) * 1000000000LL) / frequency.QuadPart);
}

double get_time_diff_s(high_res_time_t *start, high_res_time_t *end) {
    return (end->tv_sec - start->tv_sec) +
           (end->tv_nsec - start->tv_nsec) / 1e9;
}

// DONNEES

void remplir_aleatoire(long long T[], long long n) {
    for (long long i = 0; i < n; i++)
        T[i] = rand() % 100000;
}

//TRI PAR BASE

int cle(long long x, int i) {
    while (i--) x /= 10;
    return x % 10;
}

void TriAux(long long T[], long long n, int i) {
    long long *temp = malloc(n * sizeof(long long));
    long long cpt[10] = {0};

    for (long long j = 0; j < n; j++)
        cpt[cle(T[j], i)]++;

    for (int j = 1; j < 10; j++)
        cpt[j] += cpt[j - 1];

    for (long long j = n - 1; j >= 0; j--)
        temp[--cpt[cle(T[j], i)]] = T[j];

    for (long long j = 0; j < n; j++)
        T[j] = temp[j];

    free(temp);
}

double TriBase(long long T[], long long n, int k) {
    high_res_time_t start, end;
    get_high_precision_time(&start);

    for (int i = 0; i < k; i++)
        TriAux(T, n, i);

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

// TRI RAPIDE

int partitionner(long long tab[], int d, int f) {
    long long pivot = tab[d];
    int i = d + 1, j = f;

    while (1) {
        while (i <= f && tab[i] <= pivot) i++;
        while (tab[j] > pivot) j--;
        if (i < j) {
            long long tmp = tab[i];
            tab[i] = tab[j];
            tab[j] = tmp;
        } else break;
    }
    tab[d] = tab[j];
    tab[j] = pivot;
    return j;
}

void triRapideRec(long long tab[], int p, int r) {
    if (p < r) {
        int q = partitionner(tab, p, r);
        triRapideRec(tab, p, q - 1);
        triRapideRec(tab, q + 1, r);
    }
}

double triRapide(long long tab[], int n) {
    high_res_time_t start, end;
    get_high_precision_time(&start);

    triRapideRec(tab, 0, n - 1);

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

// TRI PAR TAS

void entasser(long long tab[], int taille, int i) {
    int plusGrand = i;
    int g = 2*i + 1;
    int d = 2*i + 2;

    if (g < taille && tab[g] > tab[plusGrand]) plusGrand = g;
    if (d < taille && tab[d] > tab[plusGrand]) plusGrand = d;

    if (plusGrand != i) {
        long long tmp = tab[i];
        tab[i] = tab[plusGrand];
        tab[plusGrand] = tmp;
        entasser(tab, taille, plusGrand);
    }
}

double triParTas(long long tab[], int n) {
    high_res_time_t start, end;
    get_high_precision_time(&start);

    for (int i = n/2 - 1; i >= 0; i--)
        entasser(tab, n, i);

    for (int i = n - 1; i > 0; i--) {
        long long tmp = tab[0];
        tab[0] = tab[i];
        tab[i] = tmp;
        entasser(tab, i, 0);
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

//TRI BULLE

double TriBulle(long long T[], int n) {
    bool changement = true;
    high_res_time_t start, end;
    get_high_precision_time(&start);

    while (changement) {
        changement = false;
        for (int i = 0; i < n - 1; i++) {
            if (T[i] > T[i + 1]) {
                long long tmp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = tmp;
                changement = true;
            }
        }
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

double TriBulleOpt(long long T[], int n) {
    bool changement = true;
    int m = n - 1;
    high_res_time_t start, end;
    get_high_precision_time(&start);

    while (changement) {
        changement = false;
        for (int i = 0; i < m; i++) {
            if (T[i] > T[i + 1]) {
                long long tmp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = tmp;
                changement = true;
            }
        }
        m--;
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

//TRI GNOME

double triGnome(long long t[], int n) {
    high_res_time_t start, end;
    get_high_precision_time(&start);

    int i = 0;
    while (i < n - 1) {
        if (t[i] <= t[i + 1]) i++;
        else {
            long long a = t[i];
            t[i] = t[i + 1];
            t[i + 1] = a;
            if (i > 0) i--;
        }
    }

    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

//MAIN

int main() {
    srand(time(NULL));

    long long valeurs_n[7] = {10000, 20000, 50000, 100000, 200000, 500000, 1000000};

    FILE *f_base   = fopen("tri_base.csv", "w");
    FILE *f_rapide = fopen("tri_rapide.csv", "w");
    FILE *f_tas    = fopen("tri_tas.csv", "w");
    FILE *f_bulle  = fopen("tri_bulle.csv", "w");
    FILE *f_bulleO = fopen("tri_bulle_opt.csv", "w");
    FILE *f_gnome  = fopen("tri_gnome.csv", "w");

    fprintf(f_base,   "n,temps\n");
    fprintf(f_rapide, "n,temps\n");
    fprintf(f_tas,    "n,temps\n");
    fprintf(f_bulle,  "n,temps\n");
    fprintf(f_bulleO, "n,temps\n");
    fprintf(f_gnome,  "n,temps\n");

    for (int i = 0; i < 7; i++) {
        long long n = valeurs_n[i];
        double t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;

        for (int j = 0; j < 10; j++) {
            long long *T = malloc(n * sizeof(long long));

            remplir_aleatoire(T,n); t1 += TriBase(T,n,5);
            remplir_aleatoire(T,n); t2 += triRapide(T,n);
            remplir_aleatoire(T,n); t3 += triParTas(T,n);
            remplir_aleatoire(T,n); t4 += TriBulle(T,n);
            remplir_aleatoire(T,n); t5 += TriBulleOpt(T,n);
            remplir_aleatoire(T,n); t6 += triGnome(T,n);

            free(T);
        }

        fprintf(f_base,   "%lld,%.9f\n", n, t1/10);
        fprintf(f_rapide, "%lld,%.9f\n", n, t2/10);
        fprintf(f_tas,    "%lld,%.9f\n", n, t3/10);
        fprintf(f_bulle,  "%lld,%.9f\n", n, t4/10);
        fprintf(f_bulleO, "%lld,%.9f\n", n, t5/10);
        fprintf(f_gnome,  "%lld,%.9f\n", n, t6/10);
    }

    fclose(f_base); fclose(f_rapide); fclose(f_tas);
    fclose(f_bulle); fclose(f_bulleO); fclose(f_gnome);

    printf("Tous les CSV ont ete generes avec succes\n");
    return 0;
}

