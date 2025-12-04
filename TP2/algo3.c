#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


int rechElets_Dicho(int tab[], int n, int x){

    int deb = 0;
    int fin = n-1;
    int m;

    while (deb <= fin){
        m = (deb + fin) / 2;

        if (tab[m] == x) {
            return 1;
        }
        else if (tab[m] < x) {
            deb = m + 1;
        }
        else {
            fin = m - 1;
        }
    }
    return 0;
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
    int res = rechElets_Dicho(tab2, n2, x);
    clock_t fin = clock();

    double temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    if (res == 1)
        printf("La valeur %d existe dans le tableau.\n", x);
    else
        printf("La valeur %d n'existe pas dans le tableau.\n", x);
    printf("Temps d'exécution : %.6f s\n", temps);

    free(tab2);

}