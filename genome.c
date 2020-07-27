#include "genome.h"

// helpers
int getval(char c);
GNode *create_GNode(char *key, int key_len);
void delete_GNode(GNode **a_gnd);
void delete_ListNode(ListNode **a_nd);
void delete_List(ListNode **a_head);

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

GNode *find_GNode(UnorderedSet *set, char *key)
{
    long index = hash(set, key);
    GNode *gnd = NULL;
    ListNode *temp = set->table[index];
    while (temp->next != NULL && strcmp(key, temp->key) == 0)
    {
        temp = temp->next;
    }

    if (temp != NULL)
    {
        gnd = temp->data;
    }
    return gnd;
}

GNode *create_GNode(char *key, int key_len)
{
    GNode *gnd = malloc(sizeof(*gnd));
    if (gnd != NULL)
    {
        nd->key = malloc(sizeof(char) * (key_len + 1));
        strcpy(nd->key, key);
    }
}

int insert_GNode(UnorderedSet **a_set, char *key)
{
    GNode *gnd = find_GNode((*a_set), key);
    if (gnd == NULL)
    {
        double load_factor = (double)(*a_set)->num_elements / (*a_set)->num_buckets;

        // resize if needed
        if (load_factor > MAX_LOAD_FACTOR)
        {
            resize_table(a_set);
        }

        long index = hash(*a_set, key);
        gnd = create_GNode(key, (*a_set)->key_len);

        // create a new ListNode
        ListNode *nd = malloc(sizeof(*nd));
        if (nd != NULL)
        {
            
            *nd = (ListNode){.key = NULL, .data = gnd, .next = NULL};
            nd->key = malloc(sizeof(*(nd->key)) * ((*a_set)->key_len + 1));
            strcpy(nd->key, key);

            // link it to last if last exists
            if ((*a_set)->table[index] == NULL)
            {
                (*a_set)->table[index] = nd;
            }
            else
            {
                ListNode *temp = (*a_set)->table[index];
                (*a_set)->table[index] = nd;
                nd->next = temp;
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
        ListNode *temp = NULL;
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

void delete_GNode(GNode **a_gnd)
{
    free(*a_gnd);
    *a_gnd = NULL;
}

void delete_ListNode(ListNode **a_nd) 
{
    free((*a_nd)->key); 
    (*a_nd)->key = NULL; 
    delete_GNode(&((*a_nd)->data));
    free(*a_nd);
    *a_nd = NULL; 
}

int remove_GNode(UnorderedSet **a_set, char* key)
{
    long index = hash(*a_set, key);
    ListNode *temp = (*a_set)->table[index];
    ListNode *prev;
    while (temp->next != NULL && strcmp(key, temp->key) == 0)
    {
        prev = temp; 
        temp = temp->next;
    }

    if (temp != NULL)
    {
        prev->next = temp->next;
        delete_ListNode(&temp);
        return 1;  
    }

    return 0; 
}

void delete_List(ListNode **a_head)
{
    ListNode *curr = *a_head;
    ListNode *next;
    while (curr != NULL)
    {
        next = curr->next;
        delete_ListNode(&curr); 
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
