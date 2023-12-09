/*
 * Copyright (C) 2009 Ronny Yabar Aizcorbe <ronnycontacto@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef LIST_H
#define LIST_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include<pthread.h>
#include <Node.h>

using namespace std;

class List
{
public:
    List();
    ~List();

    void add_head(int);
    void set_lock(pthread_rwlock_t * );
    void print();
    void del_head();
    void obt_by_position(int);
    void del_value(int);
    Node *m_head;
private:
    pthread_rwlock_t *lock;
    pthread_cond_t *Writing;
    pthread_cond_t *Reading;
    
    int m_num_nodes;
};

#endif // LIST_H