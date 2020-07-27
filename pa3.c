#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "genome.h"

#define BUFFER_SIZE 255

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
        if (in_file_ptr == NULL)
        {
            return EXIT_FAILURE;
        }
        else
        {
            char *buffer = malloc(sizeof(*buffer) * BUFFER_SIZE);
            char *key = malloc(sizeof(*key) * (key_len + 1));
            // for each line in the file, read into buffer and then do graph stuff
            while (fgets(buffer, BUFFER_SIZE, in_file_ptr) != NULL)
            {
                int i = 0;
                while (buffer[i] != '\n')
                {
                    strncpy(key, buffer + i, key_len);
                    key[key_len] = '\0';

                    // insert node check if a node with this key doesn't already exist
                    // then it calls create_node which creates and initilaizes a new GNode
                    
                
                }
            }
            free(buffer);
            free(key);
            fclose(in_file_ptr);
        }
    }

    return EXIT_FAILURE;
}
