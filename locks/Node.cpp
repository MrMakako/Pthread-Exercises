#include <Node.h>

// Constructor por defecto
Node::Node()
{
    data = 0;
    next = nullptr;
}

// Constructor por parámetro

Node::Node(int data_,pthread_rwlock_t* lock)
{
    data = data_;
    next = nullptr;
    this->lock=lock;
}

// Eliminar todos los Nodos

void Node::delete_all()
{
    if (next)
        next->delete_all();
    delete this;
}

// Imprimir un Nodo

void Node::print()
{

    std::cout << data << "-> ";
}

void Node::set_next(Node *next){
    pthread_rwlock_wrlock(lock);
    this->next=next;
    pthread_rwlock_unlock(lock);
}

Node::~Node() {}