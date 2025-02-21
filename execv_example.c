#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Save in a file named execv_example.c
 * Uses execv() to run the program passed as an argument.
 */
int main(int argc, char *argv[]){
  char *newargv[] = { NULL, "hello", "world", NULL };
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Put the name of the program being executed as element 0 of the arguments
  // passed to the program to be run by execve
  newargv[0] = argv[1];

  // Now use execv to run this program
  execv(argv[1], newargv);

 /* execve() returns only on error */
  perror("execv");   
  exit(EXIT_FAILURE);
}