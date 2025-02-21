#include <stdio.h>

/**
 * Print the message "Hello $" where $ is the element at index 1 in argv
 */

int main(int argc, char *argv[]){
  printf("Hello %s!\n", argv[1]);
  return 0;
}