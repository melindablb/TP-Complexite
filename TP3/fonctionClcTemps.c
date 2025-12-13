#include <stdio.h>
#include <stdbool.h>
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