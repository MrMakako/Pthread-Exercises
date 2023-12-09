#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <semaphore.h>
#define MSG_MAX 100


int n_threads=5;

char *messages[5]={NULL,NULL,NULL,NULL,NULL};
sem_t semaphores[5];


void * Send_msg(void * rank) {
    long my_rank=(long)rank;
    long dest=(my_rank+1)%n_threads;
    ///long source =(my_rank+ n_threads-1)%n_threads;
    char *my_msg=new char[MSG_MAX];    
    std::sprintf(my_msg,"hello %ld from %ld",dest,my_rank);
    messages[dest] = my_msg;
    sem_post(&semaphores[dest]);
    //Desbloqueo hilo 2 

    //Yo me bloqeuo 
    sem_wait(&semaphores[my_rank]);
    printf("Thread %ld > %s\n", my_rank, messages[my_rank]);
    
    //while (messages[my_rank] == NULL);
    
    
    return 0;
}


int main(){
    pthread_t threads[n_threads];
    long n;
    for( n =0;n<n_threads;n++){
        pthread_create(&threads[n],NULL,Send_msg,(void*)n);
    }
    for(long i=0;i<n_threads;i++){
        pthread_join(threads[i],NULL);
    }
    for(int i=0;i<n_threads;i++){
        delete []messages[i];
    }
}