#include "priority_queue.h"

static void resize_pq(PriorityQueue **a_pq);
static void upward_heapify(PriorityQueue **a_pq);
static void downward_heapify(PriorityQueue **a_pq, size_t start);
static void delete_OutString(OutString **a_out_str);

PriorityQueue *init_PriorityQueue(size_t initial_size)
{
    //printf("inital pq size = %ld\n", initial_size); 
    PriorityQueue *pq = malloc(sizeof(*pq));
    if (pq != NULL)
    {
        *pq = (PriorityQueue){
            .items = malloc(sizeof(*(pq->items)) * initial_size),
            .capacity = -1,
            .size = 0};

        //pq->items = malloc(sizeof(*(pq->items)) * initial_size);
        if (pq->items != NULL)
        {
            for (int i = 0; i < initial_size; i++)
            {
                pq->items[i] = NULL;
            }
            pq->capacity = initial_size;
        }
    }
    return pq;
}

static void resize_pq(PriorityQueue **a_pq)
{
    //printf("**Resizing priority queue\n");
    size_t new_size = (*a_pq)->capacity * GROWTH_MULTIPLIER;
    
    // check overflow
    if (new_size <= (*a_pq)->capacity)
    {
        return;
    }
    //printf("new pq size = %ld\n", new_size);

    (*a_pq)->items = realloc((*a_pq)->items, sizeof(*a_pq)->items * new_size);
    if ((*a_pq)->items != NULL)
    {
        (*a_pq)->capacity = new_size;
    }
}

int PriorityQueue_insert(PriorityQueue **a_pq, char *str)
{
    size_t length = strlen(str) + 1;
    // create a new GraphNode
    OutString *out_str = malloc(sizeof(*out_str));
    if (out_str != NULL)
    {
        *out_str = (OutString){.length = -1, .str = NULL};

        out_str->str = malloc(sizeof(*out_str->str) * length);
        if (out_str->str != NULL)
        {
            strcpy(out_str->str, str);
            out_str->length = length;
        }

        if ((*a_pq)->size >= (*a_pq)->capacity)
        {
            //printf("size: %ld\n", (*a_pq)->size);
            //printf("cap: %ld\n", (*a_pq)->capacity);
            resize_pq(a_pq);
        }

        (*a_pq)->items[(*a_pq)->size] = out_str;
        (*a_pq)->size += 1;

        upward_heapify(a_pq);

        return 1;
    }
    return 0;
}

static void upward_heapify(PriorityQueue **a_pq)
{

    OutString *temp = (*a_pq)->items[(*a_pq)->size - 1];
    size_t idx_child = (*a_pq)->size - 1;
    size_t idx_parent = (idx_child - 1) / 2;

    while (idx_child > 0 && temp->length > (*a_pq)->items[idx_parent]->length)
    {
        if (temp->length == (*a_pq)->items[idx_parent]->length)
        {
            // if new item does comes first alphabetically
            //child       //parent
            if (strcmp(temp->str, (*a_pq)->items[idx_parent]->str) < 0)
            {
                // swap
                (*a_pq)->items[idx_child] = (*a_pq)->items[idx_parent];
                idx_child = idx_parent;
                idx_parent = (idx_child - 1) / 2;
            }
        }
        else
        {
            // swap
            (*a_pq)->items[idx_child] = (*a_pq)->items[idx_parent];
            idx_child = idx_parent;
            idx_parent = (idx_child - 1) / 2;
        }
    }

    (*a_pq)->items[idx_child] = temp;
}

OutString *top(PriorityQueue *pq)
{
    if (pq != NULL && pq->items != NULL && pq->size > 0)
    {
        return pq->items[0];
    }
    return NULL;
}

OutString *PriorityQueue_remove(PriorityQueue **a_pq)
{
    OutString *top = NULL;
    if ((*a_pq) != NULL && (*a_pq)->items != NULL && (*a_pq)->size > 0)
    {
        // save top;
        top = (*a_pq)->items[0];

        // put last item into top
        (*a_pq)->items[0] = (*a_pq)->items[(*a_pq)->size - 1];
        (*a_pq)->items[(*a_pq)->size - 1] = top;
        (*a_pq)->size -= 1;

        downward_heapify(a_pq, 0);
    }
    return top;
}

static void downward_heapify(PriorityQueue **a_pq, size_t start)
{
    if ((*a_pq)->size <= 0)
    {
        return;
    }

    OutString *temp = (*a_pq)->items[start];
    size_t i = start;
    size_t idx_left_child = 2 * i + 1;
    size_t idx_right_child = 2 * i + 2;
    size_t idx_largest_child;

    while (idx_left_child < (*a_pq)->size)
    {
        idx_largest_child = idx_left_child;

        // if right child exists
        if (idx_right_child < (*a_pq)->size)
        {
            // which child has greater length
            if ((*a_pq)->items[idx_right_child]->length >=
                (*a_pq)->items[idx_left_child]->length)
            {

                if ((*a_pq)->items[idx_right_child]->length ==
                    (*a_pq)->items[idx_left_child]->length)
                {
                    // if right child is first alphabetically
                    if (strcmp(
                            (*a_pq)->items[idx_right_child]->str,
                            (*a_pq)->items[idx_left_child]->str) < 0)
                    {
                        idx_largest_child = idx_right_child;
                    }
                }
            }
        }

        if (temp->length > (*a_pq)->items[idx_largest_child]->length)
        {
            break;
        }
        else
        {

            if (temp->length == (*a_pq)->items[idx_largest_child]->length)
            {
                // if temp comes second alphabetically
                if (strcmp(temp->str, (*a_pq)->items[idx_largest_child]->str) > 0)
                {
                    // swap
                    (*a_pq)->items[i] = (*a_pq)->items[idx_largest_child];

                    i = idx_largest_child;
                    idx_left_child = 2 * i + 1;
                    idx_right_child = 2 * i + 2;
                }
                else
                {
                    break;
                }
            }
            else
            {
                // swap
                (*a_pq)->items[i] = (*a_pq)->items[idx_largest_child];

                i = idx_largest_child;
                idx_left_child = 2 * i + 1;
                idx_right_child = 2 * i + 2;
            }
        }
    }

    (*a_pq)->items[i] = temp;
}

static void delete_OutString(OutString **a_out_str)
{
    free((*a_out_str)->str);
    (*a_out_str)->str = NULL;

    free(*a_out_str);
    *a_out_str = NULL;
}

void delete_PriorityQueue(PriorityQueue **a_pq)
{
    for (size_t i = 0; i < (*a_pq)->capacity; i++)
    {
        if ((*a_pq)->items[i] == NULL)
        {
            break;
        }
        delete_OutString(&((*a_pq)->items[i]));
    }

    free((*a_pq)->items);
    **a_pq = (PriorityQueue){.items = NULL, .size = 0, .capacity = -1};
    free(*a_pq);
    *a_pq = NULL;
}