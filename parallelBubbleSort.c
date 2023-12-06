#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct {  //struct definition of a thread argument
 int *arr;
 int start;
 int end;
} thread_args_t;


void swap(int *a, int *b) { //helper function to switch 2 elements in an array
 int temp = *a;
 *a = *b;
 *b = temp;
}


void *thread_sort(void *args) {  // implementation of bubble sort
 thread_args_t *data = (thread_args_t *) args;   //store the data of each chunk
 int n = data->end - data->start;  // range of values to loop through
 bool swapped = true;  // conditional to check if values has swapped
                       //if not then save time and break
 while (swapped) {
   swapped = false;
   for (int i = data->start; i < data->end - 1; ++i) {
     if (data->arr[i] > data->arr[i + 1]) {
       swap(&data->arr[i], &data->arr[i + 1]); //perform the swapping algorithm
       swapped = true;
     }
   }
 }


 pthread_exit(NULL);
}


void parallel_bubble_sort(int *arr, int n, int num_threads) {
 pthread_t threads[num_threads]; // init array of threads to store their ids
 thread_args_t thread_args[num_threads]; //arr of args to pass to each thread if needed


 int chunk_size = n / num_threads; //number of elements each thread takes in the array


 // Divide the array into chunks and create threads for each
 for (int i = 0; i < num_threads; ++i) {
   int start = i * chunk_size; //start index of each thread's chunk
   int end = (i == num_threads - 1) ? n : start + chunk_size; // end index of each threads chunk
     //args to pass to each thread containing start, end, and array itself
   thread_args[i].arr = arr;
   thread_args[i].start = start;
   thread_args[i].end = end;
     //during thread creation. each thread will execute thread_sort <=> bubble sort with its chunk
     //The function will also send the args and ids of each thread
   pthread_create(&threads[i], NULL, thread_sort, &thread_args[i]);
 }


   // Wait for all threads to finish
 for (int i = 0; i < num_threads; ++i) {
   pthread_join(threads[i], NULL);
 }


 // Additional sorting steps if needed (e.g., checking for fully sorted array)
}


int main() {
 //  intialization
  int n=20000; //  adjust input size as needed for the data table


 int num_threads=1; //  also adjust number of threads here


 int *arr = (int *)malloc(n * sizeof(int)); // dynamically assign size to arr


 // Initialize the array with random values
 srand(time(NULL));
 for (int i = 0; i < n; ++i) {
   arr[i] = rand() % 100;
 }


 printf("array before applying parallel bubble sort: ");
 for (int i = 0; i < n; ++i) {
   printf("%d ", arr[i]);
 }
 printf("\n");


  parallel_bubble_sort(arr, n, num_threads);  // Sort the array using pthreads


 printf("Sorted array after parallel bubble sort: ");
 for (int i = 0; i < n; ++i) {
   printf("%d ", arr[i]);
 }
 printf("\n");


 free(arr);


 return 0;
}
