#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <time.h>

int main(){
    int waiting_time = 1;
    int n = 5000;
    int run_count[n];

    for(int i=0;i<n;i++){
        run_count[i] = 0;
    }

    for(int k=0;k<10;k++){
        time_t start;
        int leaderThread = 0;
        int count = 0;
        #pragma omp parallel num_threads(n)
        {
            time_t current_start_time = time(NULL);
            if(current_start_time <= start+waiting_time)
            {
                run_count[omp_get_thread_num()] += 1;
                count++;
            }
            if(leaderThread == 0)
            {
                #pragma omp critical
                {
                    start = time(NULL);
                    leaderThread = 1;
                    printf("leader thread : %d\n", omp_get_thread_num());
                }
            }
            while(time(NULL) <= start + waiting_time){}
        }
        printf("waiting time : %d seconds \n", waiting_time);
        printf("number of players: %d\n", n);
        printf("number of people that participated in the race are %d\n", count);
        printf("number of people that missed the race are %d\n", n - count);
        printf("\n");

        if(k>=5){
            for(int j=0;j<n;j++){
                if(10 - run_count[j] >= 5)
                {
                    n--;
                }
            }
        }
    }

    FILE *fptr;
    fptr = fopen("qp1_output.txt", "w+");
    for(int i=0;i<n;i++)
    {
        fprintf(fptr, "player %d ran %d times and missed %d times\n", i, run_count[i], 10 - run_count[i]);
    }
    fclose(fptr);
}