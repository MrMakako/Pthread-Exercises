/**
 * Basado en: https://github.com/ronnyml/C-Tutorial-Series/tree/master/Listas_enlazadas
 */

#include <list.h>

using namespace std;

// Constructor por defecto

List::List()
{

    m_num_nodes = 0;
    m_head = NULL;
}
void List::set_lock(pthread_rwlock_t * lock){
    this->lock=lock;
}
// Insertar al inicio
void List::add_head(int value)
{
    //pthread_rwlock_rdlock(lock);
    Node *new_node = new Node(value,lock);
    Node *curr = m_head;
    Node*temp=nullptr;
   
    if (!m_head)
    {
        m_head = new_node;
    }
    else if(m_head->data>value){
       
        new_node->next=m_head;
        pthread_rwlock_wrlock(lock);
        m_head=new_node;
        pthread_rwlock_unlock(lock);
        

    }else
    {
      
        int count=0;
        while (curr!=nullptr && curr->data<value)
        {
            temp=curr;
            curr=curr->next;
            count++;

        }
        if(curr==nullptr || curr->data>value){
            //Write locks a nivel de nodo
            
            new_node->next=curr;
            temp->set_next(new_node);
            

           
            



        }


    }
    m_num_nodes++;
    //pthread_rwlock_unlock(lock);
}

// Eliminar al inicio
void List::del_head()
{
  
    Node *aux1 = m_head;
    m_head = m_head->next;
    aux1->next = NULL;
    m_num_nodes--;

}

void List::del_value(int value)
{
    
    Node *curr= m_head;
    Node * temp;
    if(curr->data==value){
       
        del_head();
      
    }else{
        
        while(curr!=nullptr){
            if(curr->data==value){
               
                if(curr->next==nullptr){
                    temp->next=nullptr;
                }else{
                    temp->next=curr->next;

                }
              
                break;
               
                
            }
            temp=curr;
            curr=curr->next;

        }
        curr=nullptr;
    }
    
 

}

// Obtener por posición del nodo
void List::obt_by_position(int pos)
{
    pthread_rwlock_rdlock(lock);
    Node *temp = m_head;

    for (int i; i <= m_num_nodes; i++)
    {
        if (i == pos)
        {
            temp->print();
        }
        else
        {
            temp = temp->next;
        }
    }

    pthread_rwlock_unlock(lock);
}

void List::print()
{
     pthread_rwlock_rdlock(lock);
    Node *temp = m_head;
    if (!m_head)
    {
        cout << "La Lista está vacía " << endl;
    }
    else
    {
        while (temp)
        {
            temp->print();
            if (!temp->next)
                cout << "FINAL";
            temp = temp->next;
        }
    }
     cout << "\n\n";

      pthread_rwlock_unlock(lock);
}

List::~List() {}