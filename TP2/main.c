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

// generation tableau trie avec repetition algo 2 et 3
void genereTableauTrieAvecRepetition(long long tab[], long long n) {
    tab[0] = (long long)rand() % 1000L;
    for (long long i = 1; i < n; i++) {
        tab[i] = tab[i-1] + ((long long)rand() % 5L);
    }
}

// generation tableau non trie sans repetitions algo 4 et 5
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
        else {
            tab[i] = tab[j];
        }
        tab[j] = temp;
    }
}

// generation tableau non trie avec repetitions algo 1
void genereTableauNontrie(long long tab[], long long n) {
    if (n <= 0) return;
    for (long long i = 0; i < n; i++) {
        tab[i] = (long)rand() % 100000;
    }
    for (int i = n - 1; i > 0; i--) {
        long long  j = (long)rand() % (i + 1);
        long long temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
    }
}

// --- Recherche sequentielle (tableau trie ou non) --- Algorithme 1
double rechElets_TabNonTries(long long tab[], long long n, long long x, int *trouve) {
    high_res_time_t start, end;
    get_high_precision_time(&start);
    for (long long i = 0; i < n; i++) {
        if (tab[i] == x) {
            *trouve = 1;
            get_high_precision_time(&end);
            return get_time_diff_s(&start, &end);
        }
    }
    *trouve = 0;
    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

// --- Recherche sequentielle sur tableau trie ---  Algorithme 2
double rechElets_TabTries(long long tab[], long long n, long long x, int *trouve) {
    high_res_time_t start, end;
    get_high_precision_time(&start);
    for (long long i = 0; i < n; i++) {
        if (tab[i] == x) {
            *trouve = 1;
            get_high_precision_time(&end);
            return get_time_diff_s(&start, &end);
        }
    }
    *trouve = 0;
    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

// --- Recherche dichotomique ---   Algorithme 3
double rechElets_Dicho(long long tab[], long long n, long long x, int *trouve) {
    int deb = 0;
    int fin = (int)n - 1;
    int m;
    high_res_time_t start, end;
    get_high_precision_time(&start);
    while (deb <= fin) {
        m = (deb + fin) / 2;
        if (tab[m] == x) {
            *trouve = 1;
            get_high_precision_time(&end);
            return get_time_diff_s(&start, &end);
        } else if (tab[m] < x) {
            deb = m + 1;
        } else {
            fin = m - 1;
        }
    }
    *trouve = 0;
    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

// --- Min/Max A ---    Algorithme 4
double MaxEtMinA(long long tab[], long long n, long long *Max, long long *Min, long long *CompteurA) {
    high_res_time_t start, end;
    get_high_precision_time(&start);
    *CompteurA = 0;
    *Max = *Min = tab[0];
    for (long long i = 1; i < n; i++) {
        (*CompteurA)++;
        if (*Min > tab[i]) *Min = tab[i];
        (*CompteurA)++;
        if (*Max < tab[i]) *Max = tab[i];
    }
    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

// --- Min/Max B ---    Algorithme 5
double MaxEtMinB(long long tab[], long long n, long long *Max, long long *Min, long long *CompteurB) {
    high_res_time_t start, end;
    get_high_precision_time(&start);
    *CompteurB = 0;
    if (n % 2 == 0) {
        *Max = tab[0] > tab[1] ? tab[0] : tab[1];
        *Min = tab[0] < tab[1] ? tab[0] : tab[1];
    } else {
        *Max = *Min = tab[0];
    }
    for (long long i = 0; i + 1 < n; i += 2) {
        (*CompteurB)++;
        long long a = tab[i];
        long long b = tab[i+1];
        long long smaller = (a < b) ? a : b;
        long long larger  = (a < b) ? b : a;
        if (smaller < *Min) *Min = smaller;
        if (larger  > *Max) *Max = larger;
    }
    if (n % 2 != 0) {
        long long last = tab[n - 1];
        if (last < *Min) *Min = last;
        if (last > *Max) *Max = last;
    }
    get_high_precision_time(&end);
    return get_time_diff_s(&start, &end);
}

int main() {
    srand((unsigned)time(NULL));
    long long valeurs_n[10] = {100000,200000,400000,600000,800000,1000000,1200000,1400000,1600000,1800000};

    /*------------------------- Algorithme 1 ------------------------- */
    long long N1,X1;
    int resultat1;
    printf("-----------------------------------------------------------");
    printf("\nAlgorithme 1 \nrechElets_TabNonTries: verifie l existence d une valeur X dans un tableau non trie de taille N.\nVeuillez introduire la taille N du tableau: ");
    scanf("%lld",&N1);
    // generation du tableau
    long long *tab1 = malloc(N1 * sizeof(long long));
    genereTableauNontrie(tab1, N1);
    for (long long i = 0; i < N1; i++) {
        printf("| %lld |", tab1[i]);
    }
    printf("\n\nVeuillez introduire la valeur X dont vous voulez verifier l existence dans le tableau: ");
    scanf("%lld",&X1);
    rechElets_TabNonTries(tab1, N1, X1, &resultat1);
    printf("\nLa valeur %lld %s",X1,(resultat1==1)? "existe dans le tableau." : "n existe pas dans le tableau.");
    free(tab1);

    /*------------------------- Algorithme 2 ------------------------- */
    long long N2,X2;
    int resultat2;
    printf("\n\n-----------------------------------------------------------");
    printf("\nAlgorithme 2 \nrechElets_TabTries: verifie l existence d une valeur X dans un tableau trie de taille N.\nVeuillez introduire la taille N du tableau: ");
    scanf("%lld",&N2);
    // generation du tableau
    long long *tab2 = malloc(N2 * sizeof(long long));
    genereTableauTrieAvecRepetition(tab2, N2);
    for (long long i = 0; i < N2; i++) {
        printf("| %lld |", tab2[i]);
    }
    printf("\n\nVeuillez introduire la valeur X dont vous voulez verifier l existence dans le tableau: ");
    scanf("%lld",&X2);
    rechElets_TabNonTries(tab2, N2, X2, &resultat2);
    printf("\nLa valeur %lld %s",X2,(resultat2==1)? "existe dans le tableau." : "n existe pas dans le tableau.");
    free(tab2);

    /*------------------------- Algorithme 3 ------------------------- */
    long long N3,X3;
    int resultat3;
    printf("\n\n-----------------------------------------------------------");
    printf("\nAlgorithme 3 \nrechElets_Dicho: verifie l existence d une valeur X dans un tableau trie de taille N en utilisant la methode dichotomique.\nVeuillez introduire la taille N du tableau: ");
    scanf("%lld",&N3);
    // generation du tableau
    long long *tab3 = malloc(N3 * sizeof(long long));
    genereTableauTrieAvecRepetition(tab3, N3);
    for (long long i = 0; i < N3; i++) {
        printf("| %lld |", tab3[i]);
    }
    printf("\n\nVeuillez introduire la valeur X dont vous voulez verifier l existence dans le tableau: ");
    scanf("%lld",&X3);
    rechElets_TabNonTries(tab3, N3, X3, &resultat3);
    printf("\nLa valeur %lld %s",X3,(resultat3==1)? "existe dans le tableau." : "n existe pas dans le tableau.");
    free(tab3);

    /*------------------------- Algorithme 4 ------------------------- */
    long long N4,MAXA,MINA,COMPa;
    printf("\n\n-----------------------------------------------------------");
    printf("\nAlgorithme 4 \nMaxEtMinA: recherche des valeurs maximum et minimum dans un tableau non trie de taille N.\nLes valeurs du tableaux sont distinctes.\nVeuillez introduire la taille N du tableau: ");
    scanf("%lld",&N4);
    // generation du tableau
    long long *tab4 = malloc(N4 * sizeof(long long));
    genereTableauSansRepetition(tab4, N4);
    for (long long i = 0; i < N4; i++) {
        printf("| %lld |", tab4[i]);
    }
    MaxEtMinA(tab4, N4, &MAXA, &MINA,&COMPa);
    printf("\nValeur maximum: %lld \t Valeur minimum: %lld.",MAXA,MINA);
    free(tab4);

    /*------------------------- Algorithme 5 ------------------------- */
    long long N5,MAXB,MINB,COMPb;
    printf("\n\n-----------------------------------------------------------");
    printf("\nAlgorithme 5 \nMaxEtMinB: recherche des valeurs maximum et minimum dans un tableau non trie de taille N.\nLa recherche est realisee en suivant le principe decrit dans le support du TP.\nLes valeurs du tableaux sont distinctes.\nVeuillez introduire la taille N du tableau: ");
    scanf("%lld",&N5);
    // generation du tableau
    long long *tab5 = malloc(N5 * sizeof(long long));
    genereTableauSansRepetition(tab5, N5);
    for (long long i = 0; i < N5; i++) {
        printf("| %lld |", tab5[i]);
    }
    MaxEtMinA(tab5, N5, &MAXB, &MINB,&COMPb);
    printf("\nValeur maximum: %lld \t Valeur minimum: %lld.\n\n",MAXB,MINB);
    free(tab5);

    /*------------------------- Generation des CSV -------------------------*/
    FILE *f_partieA = fopen("partieA.csv", "w");
    FILE *f_partieB = fopen("partieB.csv", "w");
    if (!f_partieA || !f_partieB) {
        perror("fopen");
        return 1;
    }

    // entetes CSV
    fprintf(f_partieA, "Algo,N,WorstCase,BestCase\n");
    fprintf(f_partieB, "Algo,N,Temps,Compteur\n");

    for (int i = 0; i < 10; i++) {
        long long n = valeurs_n[i];
        // --- Algo 1: recherche sequentielle tableau non trie ---
        {
            long long *tab_sorted = malloc(n * sizeof(long long));
            genereTableauTrieAvecRepetition(tab_sorted, n);
            int idx;
            double worst = rechElets_TabNonTries(tab_sorted, n, -1, &idx);
            double best  = rechElets_TabNonTries(tab_sorted, n, tab_sorted[0], &idx);
            fprintf(f_partieA, "1,%lld,%.9f,%.9f\n", n, worst, best);
            free(tab_sorted);
        }

        // --- Algo 2: recherche sequentielle tableau trie ---
        {
            long long *tab_sorted = malloc(n * sizeof(long long));
            genereTableauTrieAvecRepetition(tab_sorted, n);
            int trouve;
            double worst = rechElets_TabTries(tab_sorted, n, tab_sorted[n-1]+1, &trouve);
            double best  = rechElets_TabTries(tab_sorted, n, tab_sorted[n/2], &trouve);
            fprintf(f_partieA, "2,%lld,%.9f,%.9f\n", n, worst, best);
            free(tab_sorted);
        }

        // --- Algo 3: recherche dichotomique ---
        {
            long long *tab_sorted2 = malloc(n * sizeof(long long));
            genereTableauTrieAvecRepetition(tab_sorted2, n);
            int trouve;
            double worst = rechElets_Dicho(tab_sorted2, n, tab_sorted2[n-1]+1000, &trouve);
            double best  = rechElets_Dicho(tab_sorted2, n, tab_sorted2[n/2], &trouve);
            fprintf(f_partieA, "3,%lld,%.9f,%.9f\n", n, worst, best);
            free(tab_sorted2);
        }

        // --- Algo Min/Max A et B ---
        {
            long long *tab_unsorted = malloc(n * sizeof(long long));
            genereTableauSansRepetition(tab_unsorted, n);

            long long MaxA, MinA, CompA;
            double tA = MaxEtMinA(tab_unsorted, n, &MaxA, &MinA, &CompA);
            fprintf(f_partieB, "A,%lld,%.9f,%lld\n", n, tA, CompA);

            long long MaxB, MinB, CompB;
            double tB = MaxEtMinB(tab_unsorted, n, &MaxB, &MinB, &CompB);
            fprintf(f_partieB, "B,%lld,%.9f,%lld\n", n, tB, CompB);

            free(tab_unsorted);
        }
    }

    fclose(f_partieA);
    fclose(f_partieB);
    printf("\n\n-----------------------------------------------------------");
    printf("Fichiers partieA.csv et partieB.csv generes avec succes.\n");
    return 0;
}