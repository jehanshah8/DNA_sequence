#ifndef __UNORDERED_SET_H_
#define __UNORDERED_SET_H_

#include <stdlib.h>
#include <stdio.h>

#include "dynamic_array.h"

#define INITIAL_SIZE 16
#define MAX_LOAD_FACTOR 1.5
// struct for the table
typedef struct
{
    DynamicArray *table;
    size_t num_elements;
    size_t num_buckets;

} UnorderedSet;

UnorderedSet *init_UnorderedSet(int size);
long hash(UnorderedSet *set, char *key, int key_len);
void UnorderedSet_insert(UnorderedSet *set, char *key, int key_len);
void UnorderedSet_remove(UnorderedSet* set, GNode *nd);
GNode *find(UnorderedSet *set, char *key, int key_len);
void delete_UnorderedSet(UnorderedSet *set);

#endif //__UNORDERED_SET_H_