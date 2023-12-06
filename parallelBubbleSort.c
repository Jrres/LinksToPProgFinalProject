#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
  int *arr;
  int start;
  int end;
} thread_args_t;

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void *thread_sort(void *args) {
  thread_args_t *data = (thread_args_t *) args;
  int n = data->end - data->start;
  bool swapped = true;

  while (swapped) {
    swapped = false;
    for (int i = data->start; i < data->end - 1; ++i) {
      if (data->arr[i] > data->arr[i + 1]) {
        swap(&data->arr[i], &data->arr[i + 1]);
        swapped = true;
      }
    }
  }

  pthread_exit(NULL);
}

void parallel_bubble_sort(int *arr, int n, int num_threads) {
  pthread_t threads[num_threads];
  thread_args_t thread_args[num_threads];

  int chunk_size = n / num_threads;

  // Divide the array into chunks and create threads for each
  for (int i = 0; i < num_threads; ++i) {
    int start = i * chunk_size;
    int end = (i == num_threads - 1) ? n : start + chunk_size;

    thread_args[i].arr = arr;
    thread_args[i].start = start;
    thread_args[i].end = end;

    pthread_create(&threads[i], NULL, thread_sort, &thread_args[i]);
  }

  // Wait for all threads to finish
  for (int i = 0; i < num_threads; ++i) {
    pthread_join(threads[i], NULL);
  }

  // Additional sorting steps if needed (e.g., checking for fully sorted array)
}

int main() {
  int n=20000;
  int num_threads=1;

  int *arr = (int *)malloc(n * sizeof(int));

  // Initialize the array with random values
  srand(time(NULL));
  for (int i = 0; i < n; ++i) {
    arr[i] = rand() % 100;
  }

  printf("Unsorted array: ");
  for (int i = 0; i < n; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // Sort the array using pthreads
  parallel_bubble_sort(arr, n, num_threads);

  printf("Sorted array: ");
  for (int i = 0; i < n; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  free(arr);

  return 0;
}
