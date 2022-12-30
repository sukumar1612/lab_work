#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define SEQUENTIAL_LIMIT 100

void merge(int a[], int low, int mid, int high){
    int const arr1_len = mid - low + 1;
    int const arr2_len = high - mid;
    
    int arr1[arr1_len];
    int arr2[arr2_len];
    for(int i=0;i<arr1_len;i++)
    {
        arr1[i] = a[i + low];
    }
    for(int i=0;i<arr2_len;i++)
    {
        arr2[i] = a[i + mid + 1];
    }
    int i=0,j=0,k=low;
    while(i<arr1_len && j<arr2_len){
        if(arr1[i] > arr2[j]){
            a[k] = arr2[j];
            k++;
            j++;
        }else{
            a[k] = arr1[i];
            k++;
            i++;
        }
    }
    while(i<arr1_len){
        a[k] = arr1[i];
        k++;
        i++;
    }
    while(j<arr2_len){
        a[k] = arr2[j];
        k++;
        j++;
    }
}

void mergesort(int a[], int low, int high){
    if(low >= high){
        return;
    }
    int mid = low + (high - low) / 2;

    if(high-low < SEQUENTIAL_LIMIT){
        mergesort(a, low, mid);
        mergesort(a, mid+1, high);
        merge(a, low, mid, high);
    }
    else{
        #pragma omp task
        mergesort(a, low, mid);
        #pragma omp task
        mergesort(a, mid+1, high);
        #pragma omp taskwait
        merge(a, low, mid, high);
    }
}

int main(){
    int n=1000000;
    int a[n];
    for(int i=0;i<n;i++){
        a[i] = rand()%100000;
    }

    mergesort(a, 0, n-1);
    // for(int i=0;i<n;i++){
    //     printf("%d,%d\n", a[i], i);
    // }

    return 0;
}