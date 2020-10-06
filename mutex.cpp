#include <iostream>
#include <pthread.h>
#include <math.h>
using namespace std;

#define NUM_THREADS 4

int flag = 0;
double sum = 0;
int n = pow(10,8);
pthread_mutex_t mutex;

void* Thread_sum(void* rank) {
    int *my_rank_ptr = (int *)rank;
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = n/NUM_THREADS;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    double long my_sum = 0.0;
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;
    //for (i=my_first_i; i<my_last_i; i++,factor=-factor){
    //Cambie de linea porque de esta manera el calculo de pi se sigue respetando con el otro for cuando se usa 1000 en n queda 0.
    for(i=0; i<(0xFFFFFFFF);i++,factor=-factor){
        my_sum+=factor/(2*i+1);
        //cout<<factor/(2*i+1)<<endl;
    }
    pthread_mutex_lock(&mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&mutex);
    //cout<<"\t\t\t\t"<<sum<<endl;
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t ids[NUM_THREADS];
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_create(&ids[i], NULL, &Thread_sum, &i);
    }
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_join(ids[i], NULL);
    }
    cout<<"Pi: "<<sum<<endl;
}
