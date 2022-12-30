#include<stdio.h>
#include<stdlib.h>

void swap(int *x, int *y) {

   int temp;
   temp = *x;    
   *x = *y;      
   *y = temp;
   return;
}

void bubble_sort(int a[], int n){
    int sorted = 0;
    while(!sorted){
        sorted = 1;
        #pragma omp parallel
        {
            #pragma omp for
            for(int i=0;i<n-1;i+=2){
                if(a[i] < a[i+1])
                {
                    swap(&a[i], &a[i+1]);
                    sorted=0;
                }
            }
        }
        #pragma omp parallel
        {
            #pragma omp for
            for(int i=1;i<n-1;i+=2){
                if(a[i] < a[i+1])
                {
                    swap(&a[i], &a[i+1]);
                    sorted=0;
                }
            }
        }
    }
}

int main(){
    int n=10000;
    int a[n];
    for(int i=0;i<n;i++){
        a[i] = rand()%100000;
    }
    bubble_sort(a, n);
    // for(int i=0;i<n;i++){
    //     printf("%d %d\n", a[i], i);
    // }
    return 0;
}