#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include<iostream>
#define ARRAY_SIZE 20
#define NUM_HILOS 4

double array[ARRAY_SIZE] = {1.3,2.9,0.4,0.3,1.3,4.4,1.7,0.4,3.2,0.3,4.9,2.4,3.1,4.4,3.9,0.4,4.2,4.5,4.9,0.9};
int frequency_count[ARRAY_SIZE] = {0};
int barras[]={0,1,2,3,4};
int flag=0;
std::string graf[]={"","","","",""};
void* threadFunction(void* arg) {
    int thread_id = *((int*)arg);
    int freq[]={0,0,0,0,0};

    int chunk_size = ARRAY_SIZE / NUM_HILOS;
    int inicio = thread_id * chunk_size;
    int final = (thread_id == NUM_HILOS - 1) ? (ARRAY_SIZE) : ((thread_id + 1) * chunk_size);
    for (int i = inicio; i < final; ++i) {
        int numero_floor = (int)floor(array[i]); // Apply floor to each element
        // Count frequency of each distinct number
        for (int j = 0; j < ARRAY_SIZE; ++j) {
            if (floor(array[j]) == numero_floor) {
                freq[numero_floor]++;
            }  
        }

    }
    //Busy Waiting
    while(flag!=thread_id);
    //Altermaos el array global con la suma
    for(int i=0;i<5;i++){
        for(int j=0;j<freq[i];j++){
            //utulizar funciona de concatenate
            graf[i]+="**";
        }
    }
    flag = (flag + 1) % 5;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_HILOS];
    int thread_ids[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_HILOS; ++i) {
        pthread_join(threads[i], NULL);
    }

  
    printf("Histograma de Frecuecuencias\n");
    for (int i = 0; i < 5; ++i) {
       std::cout<<graf[i]<<"\n";
    }

    return 0;
}

//d_create()

//double  array[20]={1.3,2.9,0.4,0.3,1.3,4.4,1.7,0.4,3.2,0.3,4.9,2.4,3.1,4.4,3.9,0.4,4.2,4.5,4.9,0.9};
