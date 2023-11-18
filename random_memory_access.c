#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ARRAY_SIZE 100000
#define CACHE_SIZE 64

void randomAccess(int* array, int* indices) {
    // Accessing elements randomly
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[indices[i]] *= 2;
    }
}

int random_memory_access() {
    srand((unsigned int)time(NULL));

    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time, end_time;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    // Allocate an array
    int* array = (int*)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i;
    }

    int* indices = (int*)malloc(ARRAY_SIZE * sizeof(int));
    if (indices == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(array);
        return 1;
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        indices[i] = rand() % ARRAY_SIZE;
    }

    randomAccess(array, indices);

    QueryPerformanceCounter(&end_time);
    double execution_time = ((double)(end_time.QuadPart - start_time.QuadPart)) / frequency.QuadPart;

    printf("Random access execution time: %f seconds\n", execution_time);

    free(array);
    free(indices);

    return 0;
}
