#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "genome.h"

#define MAX_KEY_LENGTH 12
#define BUFFER_SIZE 7
#define INITIAL_GRAPH_SIZE 12
#define INITIAL_PQ_SIZE 12

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
            printf("Opened file to read\n");
            UnorderedSet *graph = init_UnorderedSet(INITIAL_GRAPH_SIZE, key_len);
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
            int num_lines;
            int buffer_size = BUFFER_SIZE;
            int in_str_len = 0;
            // for each line
            for (num_lines = 0; fgets(buffer, buffer_size, in_file_ptr) != NULL; num_lines++)
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

                    is_unique += insert_GraphNode(&graph, key);
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
            printf("%d lines read\n", num_lines);
            fclose(in_file_ptr);
            fclose(pc_file_ptr);
            free(key);
            free(prev_key);

            // traversals
            const char options[] = {'A', 'C', 'G', 'T'};
            int num_traversals = 0;
            GraphNode *head = NULL;
            GraphNode *curr_nd = NULL;
            GraphNode *next_nd = NULL;
            char *next_key = malloc(sizeof(*next_key) * (key_len + 1));
            int traversal_length = 0;

            printf("Beginning traversal\n");
            //printf("key    in    out\n");
            for (int bucket = 0; bucket < graph->num_buckets; bucket++)
            {
                head = graph->table[bucket];
                while (head != NULL)
                {
                    //printf("%s     %d     %d\n", head->key, head->in_degree, head->out_degree);

                    if (is_hub_node(head) == 1 && head->out_degree > 0)
                    {
                        //printf("\t%s is a hub node\n", head->key);
                        num_traversals += 1;
                        curr_nd = head;

                        buffer[0] = curr_nd->key[0];

                        for (int edge = 0; edge < 4; edge++)
                        {
                            traversal_length = 1;
                            // if the corresponding out_edge exists
                            if (curr_nd->out_edges[edge] == options[edge])
                            {
                                // calculate partial key of next node
                                strncpy(next_key, head->key + 1, key_len - 1);
                                next_key[key_len] = '\0';
                                // complete key for each of the out edges that exist
                                next_key[key_len - 1] = options[edge];
                                //printf("\tnext_key: %s\n", next_key);

                                next_nd = find_GraphNode(graph, next_key);

                                // while next node is not a hub node
                                while (next_nd != NULL && is_hub_node(next_nd) != 1)
                                {
                                    curr_nd = next_nd;

                                    // if the buffer is too small 
                                    if(buffer_size < traversal_length + key_len + 1)
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
                                    //printf("\tnext_key: %s\n", next_key);
                                    next_nd = find_GraphNode(graph, next_key);
                                }
                                // end traversal and write last chars of the final key to string

                                for (int i = 0; i < graph->key_len; i++)
                                {
                                    buffer[traversal_length] = next_nd->key[i];
                                    traversal_length++;
                                }
                                buffer[traversal_length] = '\0';
                                traversal_length++;
                                //printf("\ttraversal length: %d\n", traversal_length);
                                printf("\t%s\n", buffer);
                                next_nd = NULL;
                                curr_nd = head;
                            }
                        }
                    }
                    head = head->next;
                }
            }

            free(buffer);
            free(next_key);
            delete_UnorderedSet(&graph);
            printf("Goodbye!\n");
        }
    }

    return EXIT_FAILURE;
}
