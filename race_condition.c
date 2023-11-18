#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ARRAY_SIZE 100000
#define CACHE_SIZE 64
#define NUM_THREADS 2

// Shared data structure
struct ThreadData {
    int* array;
    CRITICAL_SECTION* criticalSection;
};

void demoRaceCondition(int* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        // Simulate a non-atomic operation without synchronization
        array[i] += 1;
    }
}

// Thread function
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    struct ThreadData* data = (struct ThreadData*)lpParam;

    EnterCriticalSection(data->criticalSection);

    demoRaceCondition(data->array);

    LeaveCriticalSection(data->criticalSection);

    return 0;
}

int race_condition() {
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

    // Critical section for synchronization
    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);

    // Array of thread handles
    HANDLE threadHandles[NUM_THREADS];

    // Thread data
    struct ThreadData threadData[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].array = array;
        threadData[i].criticalSection = &criticalSection;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadHandles[i] = CreateThread(NULL, 0, ThreadFunction, &threadData[i], 0, NULL);
        if (threadHandles[i] == NULL) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(NUM_THREADS, threadHandles, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < NUM_THREADS; ++i) {
        CloseHandle(threadHandles[i]);
    }

    // Destroy the critical section
    DeleteCriticalSection(&criticalSection);

    QueryPerformanceCounter(&end_time);
    double execution_time = ((double)(end_time.QuadPart - start_time.QuadPart)) / frequency.QuadPart;

    printf("Race condition execution time: %f seconds\n", execution_time);

    free(array);

    return 0;
}
