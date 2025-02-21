#include <stdio.h>
#include <stdlib.h>

/*
 * Save in a file named myecho.c
 * Print out the arguments passed in argv
 */
int main(int argc, char *argv[]){
  int j;
  for (j = 0; j < argc; j++){
    printf("argv[%d]: %s\n", j, argv[j]);
 }
  exit(EXIT_SUCCESS);
}