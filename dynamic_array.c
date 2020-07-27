#include "dynamic_array.h"

void _delete_List(ListNode **head);

DynamicArray *init_DynamicArray(size_t capacity)
{
    DynamicArray *da = malloc(sizeof(*da));
    if (da != NULL)
    {
        *da = (DynamicArray){.arr = NULL, .capacity = -1, .size = 0};

        da->arr = malloc(sizeof(*(da->arr)) * capacity);
        if (da->arr != NULL)
        {
            memset(da->arr, NULL, capacity);
            da->capacity = capacity;
            da->size = 0;
        }
    }
    return da;
}

int DynamicArray_insert(DynamicArray *da, long index, GNode *item)
{
    // create a new node
    ListNode *nd = malloc(sizeof(*nd));
    if (nd != NULL)
    {
        *nd = (ListNode){.data = item, .next = NULL, .last = NULL};

        // link it to last if last exists
        if (da->arr[index] == NULL)
        {
            da->arr[index] = nd;
            da->arr[index]->last = nd;
        }
        else
        {
            da->arr[index]->last->next = nd;
            da->arr[index]->last = nd;
        }
        da->size += 1;
        return 1;
    }
    return 0;
}

int DynamicArray_remove(DynamicArray *da, long index, GNode *item)
{
    ListNode *temp = da->arr[index];
    ListNode *prev = NULL;

    // If head node contains iteam to be deleted
    if (temp != NULL && strcmp(item->key, temp->data->key) == 0)
    {
        da->arr[index] = temp->next;
        delete_GNode(item);
        free(temp);
        da->size -= 1;
        return 1;
    }

    while (temp != NULL && strcmp(item->key, temp->data->key) == 0)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        return 0;
    }

    prev->next = temp->next;
    delete_GNode(item);
    free(temp);
    da->size -= 1;
    return 1;
}

DynamicArray *resize(DynamicArray *da)
{
    da->arr = realloc(da->arr, sizeof(*(da->arr)) * da->capacity * GROWTH_MULTIPLIER);
    if (da->arr)
    {
        /**
        for (int i = da->capacity; i < da->capacity * GROWTH_MULTIPLIER; i++)
        {
            da->arr[i] = NULL;
        }
        */
        memset(da->arr + da->capacity, NULL, da->capacity * GROWTH_MULTIPLIER);
        da->capacity *= GROWTH_MULTIPLIER; 
        return da;
    }
    else
    { // if realloc failed, the original pointer needs to be freed
        free(da->arr);
    }
}

void delete_List(ListNode **head)
{
    ListNode *temp;
    while (*head != NULL)
    {
        temp = (*head)->next;
        delete_GNode((*head)->data);
        free(*head);
        *head = temp;
    }
    *head = NULL;
}

void delete_DynamicArray(DynamicArray *da)
{
    for (int i = 0; i < da->size; i++)
    {
        _delete_List(&(da->arr[i]));
    }

    free(da->arr);
    *da = (DynamicArray){.arr = NULL, .capacity = -1, .size = 0};
    free(da);
}