#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <time.h>       
#include <unistd.h>   

#define SEQUENTIAL_LIMIT 100

void swap(int *a, int *b){
    int t=*a;
    *a=*b;
    *b=t;
}

int partition(int arr[], int high, int low){
    int pivot = arr[high];
    int i = low - 1;
    for(int j=low;j<high; j++)
    {
        if(arr[j] < pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i+1], &arr[high]);
    return i+1;
}

void quicksort_parallel(int arr[], int high, int low){
    if(low<high){
        int partition_index = partition(arr, high, low);
        if(high - low <= SEQUENTIAL_LIMIT){
             quicksort_parallel(arr, partition_index-1, low);
             quicksort_parallel(arr, high, partition_index+1);
             return;
        }

        #pragma omp parallel
        {
            #pragma omp sections
            {
                #pragma omp section
                {
                    quicksort_parallel(arr, partition_index-1, low);
                }
                #pragma omp section
                {
                    quicksort_parallel(arr, high, partition_index+1);
                }
            }
        }
    }
}

int main(){
    int n=1000000;
    int arr[n];
    for(int i=0;i<n;i++){
        arr[i] = rand()%100000;
    }
    quicksort_parallel(arr, n-1, 0);
}