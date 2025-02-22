#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    char userName[32];
    char *newargv[] = { "hello_world", NULL, NULL };
    printf("Please enter your name: ");
    scanf("%s", userName);
    // Put the userName as element 1
    newargv[1] = userName;
    // Now use execv to run "hello_world"
    execv("hello_world", newargv);
    /* execve() returns only on error */
    perror("execv");
    exit(EXIT_FAILURE);
}
