#ifndef __DYNAMIC_ARRAY_H_
#define __DYNAMIC_ARRAY_H_

#include <stdlib.h>
#include "genome.h"
#define GROWTH_MULTIPLIER 2

typedef struct _Node 
{    
   GNode* data;
   struct _Node *next;
   struct _Node *last; 
} ListNode;

typedef struct
{ 
    ListNode **arr; // Array of pointers 
    size_t capacity; // Total size of array
    size_t size; // Number of linked lists in array
} DynamicArray;

DynamicArray *init_DynamicArray(size_t capacity);
void DynamicArray_insert(DynamicArray *da, long index, GNode *item);
void DynamicArray_remove(DynamicArray *da, long index, GNode *item);
DynamicArray* resize_DynamicArray(DynamicArray* da);  
void delete_DynamicArray(DynamicArray *da);

#endif //__DYNAMIC_ARRAY_H_