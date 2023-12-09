#include<pthread.h>
#include <stdio.h>
#include <iostream>
#include "list.h"
#include <chrono>
int threads=8;

int lista[]={1,2,3,4,5,6,7,8};
pthread_rwlock_t lock;
List mi_lista = List();

void *read(void * arg ){
    int thread_id= *((int*)arg);

            

            
    mi_lista.obt_by_position(thread_id);

           
  
  


    return 0;


}

void *update(void * arg){
    int thread_id= *((int*)arg);
    
    printf("deleting %d\n" ,thread_id);
  
    mi_lista.del_value(thread_id*2);
    
    return 0;

}

void *insert(void *arg){
    
   int  thread_id= *((int*)arg);
  
   
    mi_lista.add_head(thread_id*10);
  
   
    return 0;

}
int main(){
    
    pthread_rwlock_init(&lock,NULL);
    mi_lista.set_lock(&lock);
    for(int i=0;i<20;i++){
        mi_lista.add_head(i);
    }
    auto start = std::chrono::high_resolution_clock::now();

   
    
    pthread_t Hilos[threads];
    int threadArgs[threads];
    for(int  i=0;i<threads;i++){
        threadArgs[i] =i;
      
        if(i>2 && i<6){
            pthread_create(&Hilos[i],NULL,insert,(void*)&threadArgs[i]);

        }else{
            pthread_create(&Hilos[i],NULL,read,(void*)&threadArgs[i]);
        }
        

    }
    for(int i=0;i<threads;i++){
        pthread_join(Hilos[i],NULL);
    }
    

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

  
    mi_lista.print();
    


    

    return 0;



}