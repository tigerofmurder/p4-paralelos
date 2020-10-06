#include <iostream>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

using namespace std;

#define NUM_THREADS 4
#define SHARED 1


int counter =0;
pthread_mutex_t barrier_mutex;

void barrier_simple(){
    pthread_mutex_lock(&barrier_mutex);
    counter++;
    pthread_mutex_unlock(&barrier_mutex);
    while(counter<NUM_THREADS);
}

pthread_mutex_t bar_mutex;
pthread_cond_t bar_cond;
int bar_count = 0;

/*Barrier Condition variables*/
void barrier_cond() {
	pthread_mutex_lock(&bar_mutex);
	bar_count++;
	if(bar_count == NUM_THREADS) {
		//bar_count = 0;
		pthread_cond_broadcast(&bar_cond);
	}
	else {
		while (pthread_cond_wait(&bar_cond, &bar_mutex) != 0);
	}
	pthread_mutex_unlock(&bar_mutex);
}

sem_t count_sem;
sem_t barrier_sem;

/*Barrier Semaphores*/
void barrier_sema(){
	sem_wait(&count_sem);
    if(bar_count == NUM_THREADS-1) {
		bar_count = 0;
		sem_post(&count_sem);
		for (int j=0;j<NUM_THREADS-1;j++){
			sem_post(&barrier_sem);
		}
	}
	else {
		bar_count++;
		sem_post(&count_sem);
		sem_wait(&barrier_sem);
    }
}

/*Programa que se trata de sincronizar*/

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
    
    barrier_sema();
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;
    for(i=0; i<n;i++,factor=-factor){
        my_sum+=factor/(2*i+1);
    }
    pthread_mutex_lock(&mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}


int main(){
	sem_init(&count_sem, SHARED, 1);
    sem_init(&barrier_sem, SHARED, 0);

    pthread_t ids[NUM_THREADS];
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_create(&ids[i], NULL, Thread_sum, &i);
    }
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_join(ids[i], NULL);
    }
    cout<<sum<<endl;
}
