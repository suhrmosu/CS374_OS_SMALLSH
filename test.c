#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    pid_t spawnpid = -5;
    int intVal = 10;
    // If fork is successful, the value of spawnpid will be 0 in the child
    // and will be the child's pid in the parent
    spawnpid = fork();
    switch (spawnpid){
        case -1:
            perror("fork() failed!");
            exit(1);
            break;
        case 0:
            // spawnpid is 0 in the child
            intVal = intVal + 1;
            printf("I am the child! intVal = %d\n", intVal);
            break;
        default:
            // spawnpid is the pid of the child
            intVal = intVal - 1;
            printf("I am the parent! intVal = %d\n", intVal);
            break;
   }
    printf("This statement will be executed by both of us!\n");
}