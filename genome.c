#include "genome.h"

// helpers
int getval(char c);
void delete_GraphNode(GraphNode **a_nd);
void delete_List(GraphNode **a_head);

UnorderedSet *init_UnorderedSet(size_t num_buckets, int key_len)
{
    UnorderedSet *set = malloc(sizeof(*set));
    if (set != NULL)
    {
        *set = (UnorderedSet){
            .key_len = key_len,
            .table = malloc(sizeof(*(set->table)) * num_buckets),
            .num_buckets = -1,
            .num_elements = 0};

        if (set->table != NULL)
        {
            memset(set->table, NULL, num_buckets);
            set->num_buckets = num_buckets;
        }
    }
    return set;
}

int getval(char c)
{
    switch (c)
    {
    case 'A':
    case 'a':
        return 0;
    case 'C':
    case 'c':
        return 1;
    case 'G':
    case 'g':
        return 2;
    case 'T':
    case 't':
        return 3;
    default:
        return 0;
    }
    return 0;
}

long hash(UnorderedSet *set, char *key)
{
    long index = 0;
    for (int i = 0; i < set->key_len; ++i)
    {
        index = index << 2;
        index |= getval(key[i]);
    }
    index = index % set->num_buckets;
    printf("hash for %s = %ld\n", key, set->key_len);
    return index;
}

GraphNode *find_GraphNode(UnorderedSet *set, char *key)
{
    long index = hash(set, key);
    GraphNode *temp = set->table[index];
    while (temp->next != NULL && strcmp(key, temp->key) == 0)
    {
        temp = temp->next;
    }

    return temp;
}

int insert_GraphNode(UnorderedSet **a_set, char *key)
{
    GraphNode *nd = find_GraphNode((*a_set), key);
    if (nd == NULL)
    {
        double load_factor = (double)(*a_set)->num_elements / (*a_set)->num_buckets;

        // resize if needed
        if (load_factor > MAX_LOAD_FACTOR)
        {
            resize_table(a_set);
        }

        // create a new GraphNode
        GraphNode *nd = malloc(sizeof(*nd));
        if (nd != NULL)
        {

            *nd = (GraphNode){
                .key = NULL,
                .in_degree = 0,
                .in_edges = {0, 0, 0, 0},
                .out_degree = 0,
                .out_edges = {0, 0, 0, 0},
                .next = NULL};

            nd->key = malloc(sizeof(*(nd->key)) * ((*a_set)->key_len + 1));
            strcpy(nd->key, key);

            long index = hash(*a_set, key);
            // link it to last if last exists
            if ((*a_set)->table[index] == NULL)
            {
                (*a_set)->table[index] = nd;
            }
            else
            {
                GraphNode *temp = (*a_set)->table[index];
                (*a_set)->table[index] = nd;
                nd->next = temp;
            }
            (*a_set)->num_elements += 1;

            // fix all edges
            char options[] = {'A', 'C', 'G', 'T'};
            char *temp_key = malloc(sizeof(*temp_key) * ((*a_set)->key_len + 1));

            // fix out_edges for all possible nodes that can connect to this new node
            for (int i = 0; i < (*a_set)->key_len - 1; i++)
            {
                temp_key[i + 1] = key[i];
            }
            temp_key[(*a_set)->key_len] = '\0';

            GraphNode *temp_nd;
            for (int i = 0; i < 4; i++)
            {
                temp_key[0] = options[i];
                temp_nd = find_GraphNode(*a_set, temp_key);
                if (temp_nd != NULL)
                {
                    temp_nd->out_edges[i] = options[i];
                    temp_nd->out_degree += 1; 
                    nd->in_edges[i] = options[i]; 
                    nd->in_degree += 1; 
                }
            }
            
            // fix in_edges for all possible nodes that can connect to this new node
            for (int i = 0; i < (*a_set)->key_len - 1; i++)
            {
                temp_key[i] = key[i + 1];
            }
            temp_key[(*a_set)->key_len] = '\0';

            for (int i = 0; i < 4; i++)
            {
                temp_key[(*a_set)->key_len - 1] = options[i];
                temp_nd = find_GraphNode(*a_set, temp_key);
                if (temp_nd != NULL)
                {
                    temp_nd->in_edges[i] = options[i];
                    temp_nd->in_degree += 1; 
                    nd->out_edges[i] = options[i]; 
                    nd->out_degree += 1; 
                }
            }

            return 1;
        }
        return 0;
    }
}

void resize_set(UnorderedSet **a_set)
{
    UnorderedSet *new_set = init_UnorderedSet((*a_set)->num_buckets * GROWTH_MULTIPLIER, (*a_set)->key_len);
    if (new_set != NULL)
    {
        GraphNode *temp = NULL;
        long index;
        for (int i = 0; i < (*a_set)->num_buckets; i++)
        {
            temp = (*a_set)->table[i];
            while (temp != NULL)
            {
                // make new head = next
                (*a_set)->table[i] = temp->next;

                // detach old head
                temp->next = NULL;

                // put old head in new table
                index = hash(new_set, temp->key);
                new_set->table[index] = temp;

                // make temp the new head
                temp = (*a_set)->table[i];
            }
        }
    }

    // delete old set
    delete_UnorderedSet(a_set);

    // make the address point to the new set
    *a_set = new_set;
}

void delete_GraphNode(GraphNode **a_nd)
{
    free((*a_nd)->key);
    free(*a_nd);
    *a_nd = NULL;
}

int remove_GraphNode(UnorderedSet **a_set, char *key)
{
    long index = hash(*a_set, key);
    GraphNode *temp = (*a_set)->table[index];
    GraphNode *prev;
    while (temp != NULL && strcmp(key, temp->key) == 0)
    {
        prev = temp;
        temp = temp->next;
    }

    // node found?
    if (temp != NULL)
    {
        prev->next = temp->next;
        delete_GraphNode(&temp);
        (*a_set)->num_elements -= 1;
        return 1;
    }

    return 0;
}

void delete_List(GraphNode **a_head)
{
    GraphNode *curr = *a_head;
    GraphNode *next;
    while (curr != NULL)
    {
        next = curr->next;
        delete_GraphNode(&curr);
        curr = next;
    }
    *a_head = NULL;
}

void delete_UnorderedSet(UnorderedSet **a_set)
{
    for (int i = 0; i < (*a_set)->num_buckets; i++)
    {
        delete_List(&((*a_set)->table[i]));
    }

    free((*a_set)->table);
    **a_set = (UnorderedSet){.table = NULL, .num_buckets = -1, .num_elements = 0};
    free(*a_set);
    *a_set = NULL;
}
