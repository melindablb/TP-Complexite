#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// algo5
void MaxEtMinB(int tab[], int n, int *max, int *min, long *compteur) {
    *compteur = 0;
    //si la taille est pair on compare les deux premeirs pour init le max et min sinon on prend que le 1er elmnt
    if (n % 2 == 0) {
        *max = tab[0] > tab[1] ? tab[0] : tab[1];
        *min = tab[0] < tab[1] ? tab[0] : tab[1];
    } else {
        *max = *min = tab[0];
    }

    for (int i = 0; i < n-1; i += 2) {
        (*compteur)++;
        int a = tab[i];
        int b = tab[i+1];
        if (a > b) {
            tab[i] = b;
            tab[i+1] = a;
        } else {
            tab[i] = a;
            tab[i+1] = b;
        }

        (*compteur)++;
        if (tab[i] < *min) *min = tab[i];
        (*compteur)++;
        if (tab[i+1] > *max) *max = tab[i+1];
    }

    if (n % 2 != 0) {
        if (tab[n-1] < *min) *min = tab[n-1];
        if (tab[n-1] > *max) *max = tab[n-1];
    }
}

// Fonction pour générer un tableau sans répétition (pour algo4 et 5)
void genereTableauSansRepetition(int tab[], int n) {
    int *existe = calloc(1000001, sizeof(int));
    int count = 0;
    while (count < n) {
        int val = rand() % 1000001;
        if (!existe[val]) {
            tab[count++] = val;
            existe[val] = 1;
        }
    }
    free(existe);
}

int main() {
    srand(time(NULL));
    //test algo5
    printf("\nRecherche Max et Min par paires\n");
    int n5;
    printf("Entrez la taille du tableau a generer : ");
    scanf("%d", &n5);

    int *tab5 = malloc(n5 * sizeof(int));
    if (!tab5) return 1;

    genereTableauSansRepetition(tab5, n5);

    printf("Tableau original : ");
    for (int i = 0; i < n5; i++) printf("%d ", tab5[i]);
    printf("\n");

    int max, min;
    long compteur;
    clock_t debut = clock();
    MaxEtMinB(tab5, n5, &max, &min, &compteur);
    clock_t fin = clock();
    double temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    printf("Tableau apres organisation par paires : ");
    for (int i = 0; i < n5; i++) printf("%d ", tab5[i]);
    printf("\n");

    printf("Max: %d, Min: %d, Comparaisons: %ld, Temps: %.6f s\n", max, min, compteur, temps);

    free(tab5);
    return 0;
}
