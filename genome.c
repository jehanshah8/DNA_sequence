#include "genome.h"

GNode *create_GNode(char *key, int key_len)
{
    GNode *nd = malloc(sizeof(*nd));
    if (nd == NULL)
    {
        return NULL;
    }

    nd->key_len = key_len;
    nd->key = malloc(sizeof(char) * (key_len + 1));
    strcpy(nd->key, key);
}

