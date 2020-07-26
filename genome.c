

#include <stdio.h>
#include <stdlib.h>

#include "genome.h"

int main(int argc, char* argv[]){
  // argv[1] should be an integer of what our k length should be.
  // argv[2] will be the input filename that will contain the different reads. Each line in the
  // input file corresponds to one read.
  // argv[3] will be the filename of where you should output the strings from your traversal
  if(argc != 4){
    printf("Usage: ./pa4 <k_len> <inputFilename> <outputFilename>\n");
    exit(1);
  }
  int k_len = atoi(argv[1]);
  char* inputFilename = argv[2];
  char* outputFilename = argv[3];

  /*Do some more coding here*/

  return 0;
}