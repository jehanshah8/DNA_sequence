#ifndef __UNORDERED_SET_H_
#define __UNORDERED_SET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 1.5
#define GROWTH_MULTIPLIER 2

typedef struct _Node
{
    char* key; 
    int in_degree;    // Number of edges coming in
    int in_edges[4];  // Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
    int out_degree;   // Number of edges going out
    int out_edges[4]; // Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
    struct _Node *next;
} GraphNode;

// struct for the table
typedef struct
{
    int key_len; 
    GraphNode **table;
    size_t num_elements;
    size_t num_buckets;

} UnorderedSet;

UnorderedSet *init_UnorderedSet(size_t num_buckets, int key_len);
long hash(UnorderedSet *set, char *key);
GraphNode *find_GraphNode(UnorderedSet *set, char *key);
int UnorderedSet_insert(UnorderedSet **a_set, char *key);
int UnorderedSet_remove(UnorderedSet **a_set, char* key);
void delete_UnorderedSet(UnorderedSet **a_set);

#endif //__UNORDERED_SET_H_