#include<pthread.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
struct Node
{
    Node(int value){
        data=value;
    }
    Node* next;
    int data;

    /* data */
};
int readers=0;
int writers=0;
bool writing=false;
pthread_cond_t c_writers;
pthread_cond_t c_readers;
pthread_mutex_t listMutex;
Node  head= Node(0);
int threads=8;
int member(int value, Node *head_p){
    

    int found=0;
    
    
    pthread_mutex_lock(&listMutex);
    while(writing){
        
        pthread_cond_wait(&c_writers,&listMutex);
    }
        
   

    readers++;
   

    pthread_mutex_unlock(&listMutex);
    Node * curr_p=head_p;
    while(curr_p!=nullptr ){
        if(curr_p->data==value){
           found=value;
        }
        curr_p=curr_p->next;


    }

    pthread_mutex_lock(&listMutex);
    readers--;
    if (readers == 0) {
        pthread_cond_signal(&c_writers);
    }

    pthread_mutex_unlock(&listMutex);
    return found;
   


}
void print_list(Node *head_p){
    Node *temp = head_p;
    if (!head_p)
    {
        printf("empty list \n");
    }
    else
    {
        while (temp)
        {
            printf("%d->",temp->data);
            if (!temp->next)
                printf("Final");
            temp = temp->next;
        }
    }
    
    printf("\n");

}
int insert(int value,Node*head_p){
    

    pthread_mutex_lock(&listMutex);
    
    while(writing){
        pthread_cond_wait(&c_readers,&listMutex);
    }

    pthread_mutex_unlock(&listMutex);

    writers++;
    Node *curr_p=head_p;
    Node * pred_p=nullptr;
    Node * temp_p;

    while(curr_p!=nullptr && curr_p->data<value){
        pred_p=curr_p;
        curr_p=curr_p->next;

    }
    if(curr_p==nullptr||curr_p->data>value){
        temp_p= new Node(value);
        temp_p->next=curr_p;
        if(pred_p==NULL){
            head_p=temp_p;
        }else{
            pred_p->next=temp_p;

        }
        
    }
    pthread_mutex_lock(&listMutex);
    writers--;
    
    if(writers==0){
        
        pthread_cond_signal(&c_readers);

    }
    pthread_mutex_unlock(&listMutex);
   
   

    
    return 0;
    
    
}
void* execute_selects(void* arg) {
    int  thread_id= *((int*)arg);
    for(int i=thread_id*2+10;i<thread_id*4+10;i++){
        int number=member(i,&head);
        printf("%d\n",number);
    }
   

    return 0;
}
void* execute_inserts(void* arg) {
    int  thread_id= *((int*)arg);
    for(int i=thread_id*10;i<thread_id*10+20;i++){
        insert(i,&head);
    }
    
    return 0;
}
int main(void){
    pthread_mutex_init(&listMutex, nullptr);
    pthread_cond_init(&c_readers, nullptr);
    pthread_cond_init(&c_writers, nullptr);




   for(int i=0;i<20;i++){
        insert(i,&head);
    }
    auto start = std::chrono::high_resolution_clock::now();

   

    pthread_t Hilos[threads];
    int threadArgs[threads];
    for(int  i=0;i<threads;i++){
        threadArgs[i] =i;
        if(i>1 && i<6){
            pthread_create(&Hilos[i],NULL,execute_inserts,(void*)&threadArgs[i]);

        }else{
            pthread_create(&Hilos[i],NULL,execute_selects,(void*)&threadArgs[i]);
        }
        

    }
    for(int i=0;i<threads;i++){
        pthread_join(Hilos[i],NULL);
    }
    

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    print_list(&head);
    


    

    return 0;
}