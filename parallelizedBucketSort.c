#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
typedef struct Node{
    float data;
    struct Node * next;

}Node;
void BucketSort(float arr[],int n);
void printBuckets(struct Node *list);
struct Node* SortedMerge(struct Node* a, struct Node* b);
void FrontBackSplit(struct Node* source,
                    struct Node** frontRef,
                    struct Node** backRef);
void MergeSort(struct Node** headref);
//driver function

//implementation of bucket sort 
void BucketSort(float arr[], int n) {
    struct Node** buckets;
    int j =0;
    // Initialize bucket size of array in bytes
    buckets = (struct Node**)(malloc(sizeof(struct Node*) * n));

    if (buckets != NULL) {
        // Initialize buckets in parallel
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            buckets[i] = NULL;
        }

        // Distribute elements into buckets with critical section
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            struct Node* curr = (struct Node*)malloc(sizeof(struct Node));
            float val = arr[i];
            int bpos = (int)((val / 500.0) * (n - 1));

            // Critical section to avoid race condition
#pragma omp critical
            {
                curr->data = val;
                curr->next = buckets[bpos];
                buckets[bpos] = curr; // Inserting the node
            }
        }

        // Print buckets after distribution
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            printf("Bucket[%d] after distribution: ", i);
            printBuckets(buckets[i]);
            printf("\n");
        }

        // Sort each bucket in parallel
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            MergeSort(&buckets[i]);
        }

        // Print buckets after sorting
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            printf("Bucket[%d] after sorting: ", i);
            printBuckets(buckets[i]);
            printf("\n");
        }

        // Concatenate each bucket and place into array with critical section
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            struct Node* list = buckets[i];
            while (list) {
                // Critical section to avoid race condition
#pragma omp critical
                {
                    arr[j++] = list->data;
                    list = list->next;
                }
            }
        }

        // Print the sorted array
        printf("The sorted array is:\n");
        for (int i = 0; i < n; ++i) {
            printf("%.3f ", arr[i]);
        }
        printf("\n");

        // Clean up
        for (int i = 0; i < n; ++i) {
            struct Node* current = buckets[i];
            struct Node* next;
            while (current != NULL) {
                next = current->next;
                free(current);
                current = next;
            }
        }
        free(buckets);
    } else {
        printf("Error with malloc\n");
    }
}
//use merge sort to sort each bucket. It has the fastest run time out of all the other sorting algorithms
void MergeSort(struct Node** headRef)
{
    struct Node* head = *headRef;
    struct Node* a;
    struct Node* b;
 
    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }
 
    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);
 
    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);
 
    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}
 
/* See https:// www.geeksforgeeks.org/?p=3622 for details of
this function */
struct Node* SortedMerge(struct Node* a, struct Node* b)
{
    struct Node* result = NULL;
 
    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
 
    /* Pick either a or b, and recur */
    if (a->data <= b->data) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}
 
/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back
   halves, and return the two lists using the reference
   parameters. If the length is odd, the extra node should
   go in the front list. Uses the fast/slow pointer
   strategy. */
void FrontBackSplit(struct Node* source,
                    struct Node** frontRef,
                    struct Node** backRef)
{
    struct Node* fast;
    struct Node* slow;
    slow = source;
    fast = source->next;
 
    /* Advance 'fast' two nodes, and advance 'slow' one node
     */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
 
    /* 'slow' is before the midpoint in the list, so split
    it in two at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void printBuckets(struct Node *list) {
     if (list == NULL) {
        printf("(empty)");
        return;
    }
  struct Node *cur = list;
  while (cur) {
    printf("%f ", cur->data);
    cur = cur->next;
  }
}
 
// Driver program to test above function
int main()
{
    //100k input array takes about 1.332 seconds to run
    float arr[100000];
    int n = sizeof(arr) / sizeof(arr[0]);
     int max_threads = omp_get_num_procs();
      printf("Max threads supported by the system: %d\n", max_threads);

      omp_set_num_threads(2); // Adjust the number of threads as needed

    // Initialize OpenMP
    omp_set_nested(1);
    printf("running Parallel Bucket Sort");
    for(int i =0; i < n;i++){
        arr[i] = (float)rand() / (float)RAND_MAX * 500.0f;
    }
    BucketSort(arr, n);
    return 0;
}
