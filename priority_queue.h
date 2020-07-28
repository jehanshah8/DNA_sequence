#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GROWTH_MULTIPLIER 2

// struct for each heap element keyed by length
typedef struct
{
    char *str;
    size_t length;
} OutString;

// to maintain a priority queue of output strings using an implicit heap
typedef struct
{
    OutString **items;
    size_t capacity;
    size_t size;
} PriorityQueue;

PriorityQueue *init_PriorityQueue(size_t initial_size);
int PriorityQueue_insert(PriorityQueue **a_pq, char* str);
OutString* PriorityQueue_remove(PriorityQueue **a_pq);
OutString* top(PriorityQueue *pq); 
void delete_PriorityQueue(PriorityQueue **a_pq);    

#endif //__PRIORITY_QUEUE_H_