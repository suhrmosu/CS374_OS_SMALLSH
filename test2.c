#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 

int main(){ 
  int childStatus; 
   printf("Parent process's pid = %d\n", getpid()); 
   pid_t firstChild = fork();

   if(firstChild == -1){
     perror("fork() failed!");
     exit(EXIT_FAILURE);
   } else if(firstChild == 0){
     // The first child process will execute this branch
     printf("First child's pid = %d\n", getpid());
     sleep(10);
   } else{ 
     // The parent process will executes this branch
     // Fork another child
     pid_t secondChild = fork();

     if(secondChild == -1){
       perror("fork() failed!");
       exit(EXIT_FAILURE);
     } else if(secondChild == 0){
       // The second child process will execute this branch
       printf("Second child's pid = %d\n", getpid()); sleep(10);
     } else{
       // The parent process will executes this branch
       // wait for the second child
       pid_t childPid = waitpid(secondChild, &childStatus, 0);
       printf("The parent is done waiting. The pid of child that terminated is %d\n", childPid);
     }
  } 
  printf("The process with pid %d is exiting\n", getpid());
  return EXIT_SUCCESS;
}