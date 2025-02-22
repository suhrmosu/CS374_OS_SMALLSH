#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
    Program Name: Assignment 4: SMALLSH
    Author: Malcolm Suhr
    A basic shell in C called SMALLSH. SMALLSH will implement a subset of features of well-known shells, like bash.
    Functionality includes:
      Provide a prompt for running commands
      Handle blank lines and comments, which are lines beginning with the # character
      Execute 3 commands exit, cd, and status via code built into the shell
      Execute other commands by creating new processes using a function from the exec() family of functions
      Support input and output redirection
      Support running commands in foreground and background processes
      Implement custom handlers for 2 signals, SIGINT and SIGTSTP
    // created and updated for personal credential and demonstration
    // references third party libraries work and template functionality libraries
    // CS374 course instructional material and code provided for explicit adaptation and integration into assignment
    // URL: https://canvas.oregonstate.edu/courses/1987883/assignments/9864854?module_item_id=24956222
    // provided code sourced, reviewed, and adapted for code functionality
*/

int main(int argc, char *argv[]) {
  char userName[32];
  char *newargv[] = { "hello_world", NULL, NULL }; // file, input (command), list NULL terminator
  int choice = 0;

  int childStatus; 
  printf("Parent process's pid = %d\n", getpid()); 

  // do while loop to request user desired functionality, or exit; re-request invalid integers
  do {
    // Print User Options
    printf("1. Enter cmd to process\n");
    printf("2. Exit the program\n\n");
    // Print request line and scan to integer variable
    printf("Enter a choice 1 or 2: ");
    scanf("%d", &choice);
    if ((choice < 1) || (choice > 2)) {
      printf("You entered an incorrect choice. Try again.\n\n");
    } else if ((choice == 1)) {
      printf(": ");
      scanf("%s", userName);
      // Put the userName as element 1
      newargv[1] = userName;

      pid_t firstChild = fork();

      if (firstChild == -1) {
        perror("fork() failed!");
        exit(EXIT_FAILURE);
      } else if(firstChild == 0){
        // Now use execv to run "hello_world"
        execv("hello_world", newargv);
        /* execve() returns only on error */
        perror("execv");
        exit(EXIT_FAILURE);
        // // The first child process will execute this branch
        // printf("First child's pid = %d\n", getpid());
        // sleep(10);
      } else {
        // this is to wait for the fork first child to finish/ return, passing the firstChild PID value
        // printf("Child's pid = %d\n", firstChild);
        firstChild = waitpid(firstChild, &childStatus, 0);
        // printf("waitpid returned value %d\n", firstChild);
        if(WIFEXITED(childStatus)){
          printf("Child %d exited normally with status %d\n", firstChild, WEXITSTATUS(childStatus));
        } else{
          printf("Child %d exited abnormally due to signal %d\n", firstChild, WTERMSIG(childStatus));
        }
      }


      // // Now use execv to run "hello_world"
      // execv("hello_world", newargv);
      // /* execve() returns only on error */
      // perror("execv");
      // exit(EXIT_FAILURE);

        // // Process movies in select file for each year released
        // printf("\n");       // format matching add new line before file processing request
        // selectFile();       // process select file function
        // printf("\n");       // format matching add new line following file processing
      
    } else if ((choice == 2)) {
        break;              // Exit from the program
    }
  } while ((choice != 2));

  printf("The process with pid %d is exiting\n", getpid());
  return EXIT_SUCCESS;
  // return 0;                   // Return: integer 0 confirmation success

  // printf(": ");
  // scanf("%s", userName);
  // // Put the userName as element 1
  // newargv[1] = userName;
  // // Now use execv to run "hello_world"
  // execv("hello_world", newargv);
  // /* execve() returns only on error */
  // perror("execv");
  // exit(EXIT_FAILURE);
}

  