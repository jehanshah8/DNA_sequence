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
            .table = NULL,
            .num_buckets = -1,
            .num_elements = 0};

        set->table = malloc(sizeof(*(set->table)) * num_buckets);
        if (set->table != NULL)
        {
            for (int i = 0; i < num_buckets; i++)
            {
                set->table[i] = NULL;
            }
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
    //printf("hash for %s = %ld\n", key, index);
    return index;
}

GraphNode *find_GraphNode(UnorderedSet *set, char *key)
{
    GraphNode *temp = NULL;
    if (key != NULL && strlen(key) == set->key_len)
    {
        long index = hash(set, key);
        temp = set->table[index];
        while (temp != NULL && strcmp(key, temp->key) != 0)
        {
            temp = temp->next;
        }
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
            resize_set(a_set);
        }

        // create a new GraphNode
        nd = malloc(sizeof(*nd));
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
            return 1;
        }
    }
    return 0;
}

void add_edge(UnorderedSet **a_set, char *from, char *to)
{
    // fix out edge of from
    GraphNode *temp = find_GraphNode(*a_set, from);
    if (temp != NULL)
    {
        switch (to[(*a_set)->key_len - 1])
        {
        case 'A':
            if (temp->out_edges[0] != 'A')
            {
                temp->out_edges[0] = 'A';
                temp->out_degree += 1;
            }
            break;
        case 'C':
            if (temp->out_edges[1] != 'C')
            {
                temp->out_edges[1] = 'C';
                temp->out_degree += 1;
            }
            break;
        case 'G':
            if (temp->out_edges[2] != 'G')
            {
                temp->out_edges[2] = 'G';
                temp->out_degree += 1;
            }
            break;
        case 'T':
            if (temp->out_edges[3] != 'T')
            {
                temp->out_edges[3] = 'T';
                temp->out_degree += 1;
            }
            break;
        }
    }

    // fix in edge of to
    temp = find_GraphNode(*a_set, to);
    if (temp != NULL)
    {
        switch (from[0])
        {
        case 'A':
            if (temp->in_edges[0] != 'A')
            {
                temp->in_edges[0] = 'A';
                temp->in_degree += 1;
            }
            break;
        case 'C':
            if (temp->in_edges[1] != 'C')
            {
                temp->in_edges[1] = 'C';
                temp->in_degree += 1;
            }
            break;
        case 'G':
            if (temp->in_edges[2] != 'G')
            {
                temp->in_edges[2] = 'G';
                temp->in_degree += 1;
            }
            break;
        case 'T':
            if (temp->in_edges[3] != 'T')
            {
                temp->in_edges[3] = 'T';
                temp->in_degree += 1;
            }
            break;
        }
    }
}

void resize_set(UnorderedSet **a_set)
{
    printf("Resizing graph\n");
    UnorderedSet *new_set = init_UnorderedSet((*a_set)->num_buckets * GROWTH_MULTIPLIER, (*a_set)->key_len);
    if (new_set != NULL)
    {
        GraphNode *temp = NULL;
        GraphNode *next = NULL;
        long index;
        for (int i = 0; i < (*a_set)->num_buckets; i++)
        {
            temp = (*a_set)->table[i];
            while (temp != NULL)
            {
                // make new head = next
                next = temp->next;

                // detach old head
                temp->next = NULL;

                // put old head in new table
                index = hash(new_set, temp->key);
                // link it to last if last exists
                if (new_set->table[index] == NULL)
                {
                    new_set->table[index] = temp;
                }
                else
                {
                    GraphNode *temp2 = new_set->table[index];
                    new_set->table[index] = temp;
                    temp->next = temp2;
                }

                // make temp the new head
                temp = next;
            }
            (*a_set)->table[i] = temp;
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
    GraphNode *prev = NULL;
    while (temp != NULL && strcmp(key, temp->key) != 0)
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

int is_hub_node(GraphNode *nd)
{
    return (nd->in_degree == 1 && nd->out_degree == 1) ? 0 : 1;
}

char *get_next_key(UnorderedSet *set, GraphNode *curr_nd, char *next_key)
{
    int options[] = {'A', 'C', 'G', 'T'};
    // calculate partial key of next node
    strncpy(next_key, curr_nd->key + 1, set->key_len - 1);
    next_key[set->key_len] = '\0';

    for (int i = 0; i < 4; i++)
    {
        if (curr_nd->out_edges[i] == options[i])
        {
            next_key[set->key_len - 1] = options[i];
            break;
        }
        else
        {
            next_key[set->key_len - 1] = 0;
        }
    }
    return next_key;
}