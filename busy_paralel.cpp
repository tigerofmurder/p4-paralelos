#include <iostream>
#include <pthread.h>
#include <math.h>
using namespace std;

#define NUM_THREADS 2

long long flag = 0;
double sum = 0.0;
long n = pow(10,8);

void* Thread_sum(void* rank) {
    int *my_rank_ptr = (int *)rank;
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = n/NUM_THREADS;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;
    for (i=my_first_i; i<my_last_i; i++,factor=-factor){
        cout<<i<<" "<<my_last_i<<" "<<my_rank<<endl;

        while (flag != my_rank);
        sum+=factor/(2*i+1);
        flag=(flag+1)%NUM_THREADS;
    }
    printf("Thread %d -> %f \n", *my_rank_ptr, sum);
    
    return NULL;
}

int main() {
    pthread_t ids[NUM_THREADS];
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_create(&ids[i], NULL, Thread_sum, &i);
    }
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_join(ids[i], NULL);
    }
    cout<<sum<<endl;
}
