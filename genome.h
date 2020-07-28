#ifndef __GENOME_H_
#define __GENOME_H_

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

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
int insert_GraphNode(UnorderedSet **a_set, char *key);
void add_edge(UnorderedSet** a_set, char* from, char* to);
void resize_set(UnorderedSet **a_set);
int remove_GraphNode(UnorderedSet **a_set, char* key);
void delete_UnorderedSet(UnorderedSet **a_set);
int is_hub_node(GraphNode* nd); 
char *get_next_key(UnorderedSet* set, GraphNode* curr_nd, char* next_key);

#endif //__GENOME_H_
