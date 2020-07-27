#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "genome.h"

#define BUFFER_SIZE 255
#define INITIAL_SIZE 16

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

            UnorderedSet *graph = init_UnorderedSet(INITIAL_SIZE, key_len);
            char *buffer = malloc(sizeof(*buffer) * BUFFER_SIZE);
            char *key = malloc(sizeof(*key) * (key_len + 1));

            FILE *pc_file_ptr = NULL;
            pc_file_ptr = fopen(partial_credit_filename, "w");
            if (pc_file_ptr != NULL) 
            {
                printf("ERROR: Unable to open file"); 
            }
            int is_unique;

            // for each line in the file, read into buffer and then do graph stuff
            while (fgets(buffer, BUFFER_SIZE, in_file_ptr) != NULL)
            {
                for (int i = 0; buffer[i + key_len] != '\n'; i++)
                {
                    is_unique = 0; 
                    strncpy(key, buffer + i, key_len);
                    key[key_len] = '\0';

                    is_unique += insert_GraphNode(&graph, key);
                    // check if file was opened successfully
                    if (is_unique == 1)
                    {
                        fprintf(key, "%s\n", pc_file_ptr); 
                    }
                }

            }
            fclose(pc_file_ptr);

            
            free(buffer);
            free(key);
            delete_UnorderedSet(&graph);
            fclose(in_file_ptr);
        }
    }

    return EXIT_FAILURE;
}
