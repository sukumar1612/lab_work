#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

#define N 100000

int main(){
    int num_list[N];
    int pos[N];
    int pos2[N];
    for(int i=0;i<N;i++)
    {
        num_list[i] = rand() % N;
    }
    #pragma omp parallel for
    for(int i=0;i<N;i++)
    {
        pos[i] = 0;
        for(int j=0;j<N;j++)
        {
            if((num_list[i] > num_list[j]) || (num_list[i] == num_list[j] && i>j)){
                pos[i]+=1;
            }
        }
    }

    #pragma omp parallel for
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(i == pos[j])
            {
                pos2[i] = num_list[j];
                break;
            }
        }
    }
    printf("\n");
    for(int i=0;i<N;i++)
    {
        printf("%d, ", pos2[i]);
    }
    printf("\n");
}