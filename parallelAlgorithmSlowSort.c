#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void swap(int* xp, int* yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Function that implements Slow Sort
// Function that implements Slow Sort
void slowSort(int A[], int i, int j) {
    // Base Case
    if (i >= j)
        return;

    // Middle value
    int m = (i + j) / 2;

    // Recursively call with left half
    slowSort(A, i, m);

    // Recursively call with right
    slowSort(A, m + 1, j);

    // Swap if first element
    // is lower than second
    if (A[j] < A[m]) {
        swap(&A[j], &A[m]);
    }

    // Swap if first element
    // is lower than second
    if (A[j] < A[i]) {
        swap(&A[j], &A[i]);
    }

    // Swap if middle element
    // is lower than first
    if (A[m] < A[i]) {
        swap(&A[m], &A[i]);
    }

    // Recursively call with whole
    // array except maximum element
    slowSort(A, i, j - 1);
}


// Function to print the array
void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d \n", arr[i]);
}

// Driver Code
int main() {
     int N = 200; // Adjust the size as needed

    int* arr = (int*)malloc(N * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    omp_set_nested(1);
    omp_set_num_threads(4); // Adjust the number of threads as needed

    // Initialize the array
    for (int i = 0; i < N; ++i) {
        arr[i] = (int)rand() / (float)RAND_MAX * 500.0f;
    }
    // Parallel slow sort
    #pragma omp parallel
        #pragma omp single
    slowSort(arr, 0, N - 1);

    // Display the sorted array
    printArray(arr, N);

free(arr);

    return 0;
}
