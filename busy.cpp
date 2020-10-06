#include <iostream>
#include <pthread.h>
#include <math.h>

using namespace std;

#define NUM_THREADS 4


double sum = 0;
int n = pow(10,8);
int flag = 1;

void* Thread_sum(void* rank) {
    int *my_rank_ptr = (int *)rank;
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_n = n/NUM_THREADS;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;
    
    //cout<< *my_rank_ptr << " " <<endl;
    //for (int i =my_first_i; i<my_last_i; i++,factor=-factor){
    for(i=0; i<n;i++,factor=-factor){
        my_sum+=factor/(2*i+1);
    }
    //cout<<flag<<" "<<*my_rank_ptr<<endl;
    while (flag != *my_rank_ptr){flag=*my_rank_ptr;};
    sum+=my_sum;
    flag=(flag+1)%NUM_THREADS;
    
    //printf("Thread %d -> %f \n", *my_rank_ptr, sum);
    
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
