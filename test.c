#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "unordered_set.h"
#include "priority_queue.h"

static void add_edge(UnorderedSet **a_set, char *from, char *to);
static int is_hub_node(GraphNode *nd);
static char *get_next_key(UnorderedSet *set, GraphNode *curr_nd, char *next_key);
static int power(int base, int power);

#define MAX_KEY_LENGTH 12
#define BUFFER_SIZE 128
#define INITIAL_GRAPH_SIZE 0.5
#define INITIAL_PQ_SIZE 64

const int options[] = {'A', 'C', 'G', 'T'};

int main(int argc, char **argv)
{
    if (argc == 5)
    {
        int key_len = atoi(argv[1]);
        char *input_filename = argv[2];
        char *output_filename = argv[3];
        char *partial_credit_filename = argv[4];

        FILE *in_file_ptr = NULL;
        in_file_ptr = fopen(input_filename, "r");

        // check if file was opened successfully
        if (in_file_ptr != NULL)
        {
            //printf("Opened file to read\n");
            size_t initial_graph_size = (size_t)(power(4, key_len) * 0.5); 
            //(size_t)(INITIAL_GRAPH_SIZE * power(4, key_len))
            UnorderedSet *graph = init_UnorderedSet(initial_graph_size, key_len);
            char *buffer = malloc(sizeof(*buffer) * BUFFER_SIZE);
            char *key = malloc(sizeof(*key) * (key_len + 1));
            char escape[MAX_KEY_LENGTH] = {0};
            char *prev_key = malloc(sizeof(*prev_key) * (key_len + 1));

            FILE *pc_file_ptr = NULL;
            pc_file_ptr = fopen(partial_credit_filename, "w");
            
            if (pc_file_ptr == NULL)
            {
                printf("ERROR: Unable to open file\n");
            }

            int is_unique;

            printf("Reading lines\n");
            // for each line in the file, read into buffer and then do graph stuff
            size_t num_reads;
            int buffer_size = BUFFER_SIZE;
            int in_str_len = 0;
            // for each line
            for (num_reads = 0; fgets(buffer, buffer_size, in_file_ptr) != NULL; num_reads++)
            {
                in_str_len = strlen(buffer);
                // if the whole line didn't fit in the buffer
                while (buffer[in_str_len - 1] != '\n')
                {
                    //printf("%s\n", buffer);
                    // reallocate buffer array to a larger size
                    buffer = realloc(buffer, sizeof(*buffer) * buffer_size * 2);
                    if (buffer != NULL)
                    {
                        //printf("Resizing buffer for input\n");
                        fgets(buffer + in_str_len, buffer_size + 1, in_file_ptr);
                        buffer_size = buffer_size * 2;
                        in_str_len = strlen(buffer);
                    }
                }

                //printf("buffer: %s", buffer);

                strncpy(prev_key, escape, key_len);
                prev_key[key_len] = '\0';

                // for each key in that line
                for (int i = 0; buffer[i + key_len - 1] != '\n'; i++)
                {
                    is_unique = 0;
                    strncpy(key, buffer + i, key_len);
                    key[key_len] = '\0';

                    //printf("prev_key: %s,\tkey: %s\n", prev_key, key);

                    is_unique += UnorderedSet_insert(&graph, key);
                    if (strncmp(prev_key, escape, key_len) != 0)
                    {
                        add_edge(&graph, prev_key, key);
                    }
                    if (is_unique == 1)
                    {
                        fprintf(pc_file_ptr, "%s\n", key);
                    }
                    strcpy(prev_key, key);
                }
                
            }
            printf("%ld lines read from file\n", num_reads);
            printf("Created graph with %ld nodes\n", graph->num_elements);
            fclose(in_file_ptr);
            fclose(pc_file_ptr);
            free(key);
            free(prev_key);

            // traversals
            PriorityQueue *pq = init_PriorityQueue(graph->num_elements);

            int num_traversals = 0;
            GraphNode *head = NULL;
            GraphNode *curr_nd = NULL;
            GraphNode *next_nd = NULL;
            char *next_key = malloc(sizeof(*next_key) * (key_len + 1));
            int traversal_length = 0;

            printf("Beginning traversal\n");
            
            for (size_t bucket = 0; bucket < graph->num_buckets; bucket++)
            {
                //printf("bucket %ld: key    in    out\n", bucket);
                //printf("\t\n");
                head = graph->table[bucket];
                while (head != NULL)
                {
                    //printf("\t   %s     %d     %d\n", head->key, head->in_degree, head->out_degree);

                    if (is_hub_node(head) == 1 && head->out_degree > 0)
                    {
                        //printf("\t\t%s is a hub node\n", head->key);
                        buffer[0] = head->key[0];

                        for (int edge = 0; edge < 4; edge++)
                        {
                            curr_nd = head;
                            traversal_length = 1;
                            // if the corresponding out_edge exists
                            if (curr_nd->out_edges[edge] == options[edge])
                            {
                                //printf("\t\t\tedge found at %c\n", options[edge]);
                                num_traversals += 1;
                                // calculate partial key of next node
                                strncpy(next_key, head->key + 1, key_len - 1);
                                next_key[key_len] = '\0';
                                // complete key for each of the out edges that exist
                                next_key[key_len - 1] = options[edge];
                                //printf("\t\t\t\tnext_key: %s, ", next_key);

                                next_nd = find_GraphNode(graph, next_key);

                                // while next node is not a hub node
                                while (next_nd != NULL && is_hub_node(next_nd) != 1)
                                {
                                    curr_nd = next_nd;

                                    // if the buffer is too small
                                    if (buffer_size < traversal_length + key_len + 1)
                                    {
                                        // reallocate buffer array to a larger size
                                        buffer = realloc(buffer, sizeof(*buffer) * buffer_size * 2);
                                        if (buffer != NULL)
                                        {
                                            //printf("Resizing buffer for output\n");
                                            buffer_size = buffer_size * 2;
                                        }
                                    }
                                    
                                    buffer[traversal_length] = curr_nd->key[0];
                                    traversal_length++;

                                    next_key = get_next_key(graph, curr_nd, next_key);
                                    //printf(", next_key: %s", next_key);
                                    next_nd = find_GraphNode(graph, next_key);
                                    
                                }
                                //printf("\n");
                                // end traversal and write last chars of the final key to string

                                for (int i = 0; i < graph->key_len; i++)
                                {
                                    buffer[traversal_length] = next_nd->key[i];
                                    traversal_length++;
                                }
                                buffer[traversal_length] = '\0';
                                traversal_length++;

                                //printf("stuck\n");
                                //printf("\t\t\t\ttraversal length: %d\n", traversal_length);
                                //printf("\t\t\t\t%s\n", buffer);

                                PriorityQueue_insert(&pq, buffer);

                                next_nd = NULL;
                                
                                //curr_nd = head;
                            }
                        }
                    }
                    head = head->next;
                }
            }

            printf("Completed %d traversals\n", num_traversals);
            //printf("top: %s\n", top(pq)->str);

            //printf("\nAfter sorting\n");
            FILE *out_file_ptr = NULL;
            out_file_ptr = fopen(output_filename, "w");
            if (out_file_ptr != NULL)
            {
                size_t num_writes = 0; 
                for (int i = 0; i < pq->capacity; i++)
                {
                    OutString* out = PriorityQueue_remove(&pq);  
                    if (out != NULL) 
                    {
                        num_writes++;
                        //printf("%ld: %s\n", num_writes, out->str);
                        fprintf(out_file_ptr, "%s\n", out->str);
                    }
                }
                printf("%ld lines written to file\n", num_writes); 
            }

            fclose(out_file_ptr);
            free(buffer);
            free(next_key);
            delete_PriorityQueue(&pq);
            delete_UnorderedSet(&graph);
            printf("Goodbye!\n");
        }
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

static void add_edge(UnorderedSet **a_set, char *from, char *to)
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

static int is_hub_node(GraphNode *nd)
{
    return (nd->in_degree == 1 && nd->out_degree == 1) ? 0 : 1;
}

static char *get_next_key(UnorderedSet *set, GraphNode *curr_nd, char *next_key)
{

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

static int power(int base, int power)
{
    if (base == 0)
    {
        return 0;
    }

    int result = 1;
    for (int i = 0; i < power; i++)
    {
        result *= base;
    }
    return result;
}
