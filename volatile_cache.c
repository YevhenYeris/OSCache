#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define ARRAY_SIZE 100000

void demoVolatile(volatile int* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] *= 2;
    }
}

int volatile_cache() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time, end_time;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    // Allocate a volatile array
    volatile int* array = (volatile int*)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i;
    }

    demoVolatile(array);

    QueryPerformanceCounter(&end_time);
    double execution_time = ((double)(end_time.QuadPart - start_time.QuadPart)) / frequency.QuadPart;

    printf("Volatile execution time: %f seconds\n", execution_time);

    free((void*)array);  // avoid volatile warning

    return 0;
}
