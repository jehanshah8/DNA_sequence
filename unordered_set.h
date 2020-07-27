#ifndef __UNORDERED_SET_H_
#define __UNORDERED_SET_H_

#include <stdlib.h>
#include "genome.h"
#include "dynamic_array.h"

#define INITIAL_SIZE 16
// struct for the table
typedef struct
{
    DynamicArray *table;
    size_t num_elements;
    size_t num_buckets;

} UnorderedSet;

UnorderedSet *init_UnorderedSet(int size);
long hash(UnorderedSet* set, GNode *nd);
void UnorderedSet_insert(UnorderedSet* set, GNode *nd);
void UnorderedSet_remove(UnorderedSet* set, GNode *nd);
GNode *find(char* key);
void delete_UnorderedSet(UnorderedSet *set);

#endif //__UNORDERED_SET_H_