#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// algo2
int rechercheElmntSequent(int tab[], int n, int x) {
    for (int i = 0; i < n; i++) {
        if (tab[i] == x) return 1; // trouvé
    }
    return 0; // pas trouvé
}

// Fonction pour générer un tableau trié (pour les algo 2 et 3)
void genereTableauTrieAvecRepetition(int tab[], int n) {
    for (int i = 0; i < n; i++) {
        tab[i] = rand() % 1000001;
    }
    // tri simple
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            if (tab[j] > tab[j+1]) {
                int tmp = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = tmp;
            }
        }
    }
}

int main() {
    srand(time(NULL));

    printf("Recherche dans un tableau trié (valeurs repetées possibles)\n");
    int n2;
    printf("Entrez la taille du tableau pour algo2 : ");
    scanf("%d", &n2);

    int *tab2 = malloc(n2 * sizeof(int));
    if (!tab2) return 1;

    genereTableauTrieAvecRepetition(tab2, n2);

    printf("Tableau trie : ");
    for (int i = 0; i < n2; i++) printf("%d ", tab2[i]);
    printf("\n");

    int x;
    printf("Entrez la valeur a rechercher : ");
    scanf("%d", &x);
    
    //test algo2
    clock_t debut = clock();
    int res = rechercheElmntSequent(tab2, n2, x);
    clock_t fin = clock();

    double temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    if (res == 1)
        printf("La valeur %d existe dans le tableau.\n", x);
    else
        printf("La valeur %d n'existe pas dans le tableau.\n", x);
    printf("Temps d'exécution : %.6f s\n", temps);

    free(tab2);

    return 0;
}