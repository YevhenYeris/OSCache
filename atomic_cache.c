#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ARRAY_SIZE 100000

void demoAtomic(LONG* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        InterlockedExchange(&array[i], array[i] * 2);  // Atomic exchange to access every CACHE_SIZE-th element
    }
}

int atomic_cache() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time, end_time;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    // Allocate an array of LONG for atomic operations
    LONG* array = (LONG*)malloc(ARRAY_SIZE * sizeof(LONG));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i;
    }

    demoAtomic(array);

    QueryPerformanceCounter(&end_time);
    double execution_time = ((double)(end_time.QuadPart - start_time.QuadPart)) / frequency.QuadPart;

    printf("Atomic execution time: %f seconds\n", execution_time);

    free(array);

    return 0;
}
