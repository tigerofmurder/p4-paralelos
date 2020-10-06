#include <semaphore.h>
#include <iostream>
#include <pthread.h>
#include <math.h>


using namespace std;

#define SHARED 1
#define NUM_THREADS 6

sem_t empty, full, sm;
int data;
int produce = (NUM_THREADS-1)*2;
int consume = produce/(NUM_THREADS-1);


void *Producer(void *no){
    int *thread = (int*)no;
    for(int i=0;i<produce;i++){
        sem_wait(&empty);
        sem_wait(&sm);
        data=i;
        sem_post(&sm);
        sem_post(&full);
        printf("\nProductor: %d",data);
    }
}

void *Consumer(void *no){
    int total=0;
    int *thread = (int*)no;
    
    for(int i=0;i<consume;i++){
        sem_wait(&full);
        sem_wait(&sm);
        total=total+data;
        cout<<endl<<"Thread consumidor: "<<*thread<<" con valor de productor i: "<< data;
        sem_post(&sm);
        sem_post(&empty);
    }
    cout<<"\nEl total de "<< consume <<" iteraciones por Thread."<<*thread<<" es "<<total<<endl;
}
int main(){
    
    sem_init(&empty, SHARED, 1);
    sem_init(&full, SHARED, 0);
    sem_init(&sm,SHARED,1);
    pthread_t ids[NUM_THREADS];
    for (int i=0; i < NUM_THREADS; i++) {
        if(i == 0)
            pthread_create(&ids[i], NULL, Producer, &i);
        else
            pthread_create(&ids[i], NULL, Consumer, &i);
    }
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_join(ids[i], NULL);
    }
}
