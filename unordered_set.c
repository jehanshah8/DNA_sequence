/**
#include "unordered_set.h"

int getval(char c);

UnorderedSet *init_UnorderedSet(int size)
{
    UnorderedSet *set = malloc(sizeof(*set));
    if (set != NULL)
    {
        *set = (UnorderedSet){
            .table = init_DynamicArray(INITIAL_SIZE),
            .num_buckets = -1,
            .num_elements = 0};

        if (set->table != NULL)
        {
            set->num_buckets = INITIAL_SIZE;
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

long hash(UnorderedSet *set, char *key, int key_len)
{
    long index = 0;
    for (int i = 0; i < key_len; ++i)
    {
        index = index << 2;
        index |= getval(key[i]);
    }
    index = index % set->num_buckets;
    printf("hash for %s = %ld\n", key, key_len);
    return index;
}

void UnorderedSet_insert(UnorderedSet *set, char *key, int key_len)
{
    GNode *nd = find(set, key, key_len);
    if (nd == NULL)
    {
        double load_factor = (double)set->num_elements / set->num_buckets;
        long index;

        // resize if needed
        if (load_factor > MAX_LOAD_FACTOR)
        {
            set->table = resize_DynamicArray(set->table);
            set->num_buckets *= GROWTH_MULTIPLIER;

            ListNode *temp = NULL;
            GNode *nd = NULL;
            for (int i = 0; i < set->num_buckets / GROWTH_MULTIPLIER; i++)
            {
                temp = set->table->arr[i]; 
                while (nd != NULL)
                {
                    temp = (*head)->next;
                    delete_GNode((*head)->data);
                    free(*head);
                    *head = temp;
                }
                index = hash(set, key, key_len);
                DynamicArray_insert(set->table, index, nd);
            }
        }

        index = hash(set, key, key_len);
        nd = create_GNode(key, key_len);
        set->num_elements += DynamicArray_insert(set->table, index, nd);
    }
}

void UnorderedSet_remove(UnorderedSet *set, GNode *nd)
{
    long index = hash(set, nd->key, nd->key_len);
    set->num_elements -= DynamicArray_remove(set->table, index, nd);
}

GNode *find(UnorderedSet *set, char *key, int key_len)
{
    long index = hash(set, key, key_len);
    GNode *nd = NULL;
    ListNode *temp = set->table->arr[index];
    while (temp != NULL)
    {
        if (strcmp(key, temp->data->key) == 0)
        {
            nd = temp->data;
            break;
        }
        temp = temp->next;
    }

    return nd;
}

void delete_UnorderedSet(UnorderedSet *set)
{
    delete_DynamicArray(set->table);
    *set = (UnorderedSet){.table = NULL, .num_buckets = -1, .num_elements = 0};
}
*/