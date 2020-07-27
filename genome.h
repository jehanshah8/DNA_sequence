#ifndef __GENOME_H_
#define __GENOME_H_

#include <string.h>
#include <limits.h>

typedef struct
{
    char *key;        // Will hold strings only consisting of ACGT
    int in_degree;    // Number of edges coming in
    int in_edges[4];  // Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
    int out_degree;   // Number of edges going out
    int out_edges[4]; // Edges can be A,C,G,or T. 0 = A, 1 = C, 2 = G, 3 = T
    int key_len;      // k length
} GNode;

GNode *create_GNode(char *key, int key_len);

#endif //__GENOME_H_
