#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "genome.h"

#define BUFFER_SIZE 255
#define INITIAL_GRAPH_SIZE 256
#define INITIAL_PQ_SIZE 16

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
            for(num_lines = 0; fgets(buffer, BUFFER_SIZE, in_file_ptr) != NULL; num_lines++)
            {
                //printf("%s\n", buffer);

                for (int i = 0; buffer[i + key_len] != '\n'; i++)
                {
                    is_unique = 0;
                    strncpy(key, buffer + i, key_len);
                    key[key_len] = '\0';

                    //printf("%s\n", key);
                    
                    is_unique += insert_GraphNode(&graph, key);
                    if (is_unique == 1)
                    {
                        fprintf(pc_file_ptr, "%s\n", key);
                    }
                }
            }
            printf("%d lines read\n", num_lines);
            fclose(pc_file_ptr);

            /**
            // traversals
            const char options[] = {'A', 'C', 'G', 'T'};
            int num_hub_nodes = 0;
            GraphNode *temp = NULL;
            for (int i = 0; i < graph->num_buckets; i++)
            {
                temp = graph->table[i];
                while (temp != NULL)
                {
                    if (is_hub_node(temp) == 1)
                    {
                        num_hub_nodes += 1;

                        // for each out_edge
                        for (int i = 0; i < 4; i++)
                        {   
                            // if the corresponding out_edge exists
                            if(temp->out_edges[i] == options[i])
                            {
                                //follow it and read into buffer
                            }
                        }
                    }
                    temp = temp->next;
                }
            }
            */
            free(buffer);
            free(key);
            delete_UnorderedSet(&graph);
            fclose(in_file_ptr);
            printf("Goodbye!\n");
        }
    }

    return EXIT_FAILURE;
}
