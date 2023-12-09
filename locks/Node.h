#ifndef NODE_H
#define NODE_H

#include <iostream>
#include<pthread.h>
using namespace std;


class Node
{
    public:
        Node();
        Node(int,pthread_rwlock_t*);
        ~Node();

        Node *next;
        int data;
        pthread_rwlock_t* lock;
        void delete_all();
        void print();
        void set_next(Node*);
};

#endif // NODE_H