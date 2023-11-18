#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ARRAY_SIZE 100000
#define CACHE_SIZE 64

void induceCacheDelays(int* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i * CACHE_SIZE] *= 2;
    }
}

int cache_delays() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time, end_time;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    // Allocate an array
    int* array = (int*)malloc(ARRAY_SIZE * CACHE_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < ARRAY_SIZE * CACHE_SIZE; ++i) {
        array[i] = i;
    }

    induceCacheDelays(array);

    QueryPerformanceCounter(&end_time);
    double execution_time = ((double)(end_time.QuadPart - start_time.QuadPart)) / frequency.QuadPart;

    printf("Cache delays execution time: %f seconds\n", execution_time);

    free(array);

    return 0;
}
