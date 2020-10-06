#include <iostream>
#include <pthread.h>
#include <math.h>
using namespace std;

#define NUM_THREADS 4

long flag = 0;
double sum = 0;
int n = pow(10,2);

int turn = 1;
int counter = 0;
int t;

pthread_mutex_t bar_mutex;
pthread_cond_t bar_cond;

int bar_count = 0;

struct tData {
        int me;
        int next;
};

void barrier() {
	pthread_mutex_lock(&bar_mutex);
	++bar_count;
	if(bar_count == NUM_THREADS) {
		pthread_cond_broadcast(&bar_cond);
	}
	else {
		while (pthread_cond_wait(&bar_cond, &bar_mutex) != 0);
	}
	pthread_mutex_unlock(&bar_mutex);
}

void* Thread_sum(void* rank) {
    struct tData *data;
    data = (struct tData *) rank;
    int *my_rank_ptr = (int *)rank;
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_n = n/NUM_THREADS;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    
    //barrier();
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;
    

    for(i=0; i<n;i++,factor=-factor){
        //cout<<counter<<endl;;
        counter++;
    }
    //cout<<*my_rank_ptr<<endl;
    cout<<data->me<<endl;
    while(turn != data->me);
    cout<<"Fin de proceso "<< my_rank<<" "<<counter<<endl;
    //counter++;
    turn++;
    
    /*for (int i =my_first_i; i<my_last_i; i++,factor=-factor){
    //for(i=0; i<(0xFFFFFFFF);i++,factor=-factor){
        my_sum+=factor/(2*i+1);
    }
    cout<<my_sum<<endl;
    while (flag != my_rank);
    sum+=my_sum;
    flag=(flag+1)%NUM_THREADS;
    */
    
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
    //cout<<sum<<endl;
}
