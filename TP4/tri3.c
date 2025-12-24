//
// Created by HP on 22.12.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

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

int cle(long long x, int i){

    while (i--) x=x/10;
    if (x==0) return -1;
    return x % 10;

}

/*void TriAux(int T[],int n,int i) {
  for(int j=0;j<n-1;j++){
    for(int k=0;k<n-1;k++){
      if(cle(T[k],i)>cle(T[k+1],i)){
        int temp=T[k+1];
        T[k+1]=T[k];
        T[k]=temp;
      }
    }
  }
}
*/

void remplir_aleatoire(long long T[], long long n) {
  for (long long i = 0; i < n; i++) {
    T[i] = rand() % 100000; //constante k fixée a 5
  }
}

void TriAux(long long T[], long long n, int i) {
  long long* temp = (long long*)malloc(n * sizeof(long long));
  long long cpt[10] = {0};

  for (long long j = 0; j < n; j++) {
    int chiffre = cle(T[j], i);
    cpt[chiffre]++;
  }
  for (int j = 1; j < 10; j++) {
    cpt[j] += cpt[j - 1];
  }
  for (long long j = n - 1; j >= 0; j--) {
    int chiffre = cle(T[j], i);
    long long position = cpt[chiffre] - 1;
    temp[position] = T[j];
    cpt[chiffre]--;
  }
  for (long long j = 0; j < n; j++) {
    T[j] = temp[j];
  }
  free(temp);
}

double TriBase(long long T[],long long n,int k) {
  high_res_time_t start, end;
  get_high_precision_time(&start);

  for (int i=0;i<k;i++) {
      TriAux(T,n,i);
  }
  get_high_precision_time(&end);
  return get_time_diff_s(&start, &end);
}

int main() {

  srand(time(NULL));

  long long valeurs_n[7] = {10000, 20000, 50000, 100000, 200000, 500000, 1000000 };
  int i=0;

  FILE *f = fopen("algo3.csv", "w");
  if (f == NULL) {
    printf("Erreur lors de l ouverture du fichier.\n");
    return 1;
  }
  fprintf(f,"n,temps\n");

  while (i<7) {
    printf("n numero %d\n",i+1);
    long long n =valeurs_n[i];
    double time=0.0;

    long long *T = (long long*)malloc(n*sizeof(long long));
    remplir_aleatoire(T,n);

    int j=0;
    while (j<10) {
      time=time+TriBase(T,n,5); //k fixé a 5
      j++;
    }
    time=time/10;
    fprintf(f,"%lld,%.9f\n",n,time);

    free(T);
    i++;
  }

  fclose(f);
  printf("Fichier csv cree avec succes\n");

  return 0;
}