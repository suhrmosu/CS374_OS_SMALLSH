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
#include <stdbool.h>

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
    // URL: https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-monitoring-child-processes?module_item_id=24956219
    // URL: https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-creating-and-terminating-processes?module_item_id=24956218
    // provided code sourced, reviewed, and adapted for code functionality
*/

#define INPUT_LENGTH 2048
#define MAX_ARGS		 512

/*
  struct object to compile command line character inputs 
  Adapted from provided content
  // use and adaption from personal suhrm_assignment2.c, suhrm_assignment3.c, CS374 course instructional material and code
*/
struct command_line
{
	char *argv[MAX_ARGS + 1];
	int argc;
	char *input_file;
	char *output_file;
	bool is_bg;
  bool new_ln;
};

struct command_line *parse_input();

// int main(int argc, char *argv[]) {
//   char userName[32];
//   char *newargv[] = { "hello_world", NULL, NULL }; // file, input (command), list NULL terminator
//   int choice = 0;
//   struct command_line *curr_command;

//   int childStatus; 
//   printf("Parent process's pid = %d\n", getpid()); 

//   // do while loop to request user desired functionality, or exit; re-request invalid integers
//   do {
//     // Print User Options
//     printf("1. Enter cmd to process\n");
//     printf("2. Exit the program\n\n");
//     // Print request line and scan to integer variable
//     printf("Enter a choice 1 or 2: ");
//     scanf("%d", &choice);
//     if ((choice < 1) || (choice > 2)) {
//       printf("You entered an incorrect choice. Try again.\n\n");
//     } else if ((choice == 1)) {

//       // curr_command = parse_input(); // need to wait for return ? 
//       // sleep(5); // kinda works, first passes null, then after sleep prints the input

//       // printf(": ");
//       // scanf("%s", userName);
//       // // Put the userName as element 1
//       // newargv[1] = userName;
      
//       //newargv[1] = "not ready";
//       //newargv[1] = curr_command->argv[0]; // NULL ? 
      
//       // char *newargv[] = { curr_command->argv[0], curr_command->argv[1], NULL };

//       pid_t firstChild = fork();

//       if (firstChild == -1) {
//         perror("fork() failed!");
//         exit(EXIT_FAILURE);
//       } else if(firstChild == 0){

//         char *newargv[] = { "/bin/ls", "-al", NULL }; // works instantly when passed as constant 
//         execv(newargv[0], newargv); // kinda works? when prompt is passed "/bin/ls -al" returns error (execv: Bad address), tho still prints ls of directory

//         // Now use execv to run "hello_world"
//         //execv("hello_world", newargv);

//         /* execve() returns only on error */
//         perror("execv");
//         exit(EXIT_FAILURE);
//         // // The first child process will execute this branch
//         // printf("First child's pid = %d\n", getpid());
//         // sleep(10);
//       } else {
//         // this is to wait for the fork first child to finish/ return, passing the firstChild PID value
//         // printf("Child's pid = %d\n", firstChild);
//         firstChild = waitpid(firstChild, &childStatus, 0);
//         // printf("waitpid returned value %d\n", firstChild);
//         if(WIFEXITED(childStatus)){
//           printf("Child %d exited normally with status %d\n", firstChild, WEXITSTATUS(childStatus));
//         } else{
//           printf("Child %d exited abnormally due to signal %d\n", firstChild, WTERMSIG(childStatus));
//         }
//       }


//       // // Now use execv to run "hello_world"
//       // execv("hello_world", newargv);
//       // /* execve() returns only on error */
//       // perror("execv");
//       // exit(EXIT_FAILURE);

//         // // Process movies in select file for each year released
//         // printf("\n");       // format matching add new line before file processing request
//         // selectFile();       // process select file function
//         // printf("\n");       // format matching add new line following file processing
      
//     } else if ((choice == 2)) {
//         break;              // Exit from the program
//     }
//   } while ((choice != 2));

//   printf("The process with pid %d is exiting\n", getpid());
//   return EXIT_SUCCESS;
//   // return 0;                   // Return: integer 0 confirmation success

//   // printf(": ");
//   // scanf("%s", userName);
//   // // Put the userName as element 1
//   // newargv[1] = userName;
//   // // Now use execv to run "hello_world"
//   // execv("hello_world", newargv);
//   // /* execve() returns only on error */
//   // perror("execv");
//   // exit(EXIT_FAILURE);
// }

  
/*
  Adapted from provided content

  Function parse_input
    Tokenizes a line of character-string user-input data;
    Parses the string into appropriate structure data types;
    catches special symbols <, > and & , conditionally encodes behavior;
    Arguments: 
        char* currLine: the character pointer to the line data string
        char* newDir: the character pointer to the newly created directory name string for this file
    Returns: 
      struct command_line *parse_input: the command line struct pointer is returned

  // use and adaption from personal suhrm_assignment2.c, suhrm_assignment3.c, CS374 course instructional material and code
*/
struct command_line *parse_input()
{
	char input[INPUT_LENGTH];
	struct command_line *curr_command = (struct command_line *) calloc(1, sizeof(struct command_line));

	// Get input
	printf(": ");
	fflush(stdout);
	fgets(input, INPUT_LENGTH, stdin);

  // printf("input len %ld ;;", strlen(input));
  // printf("input[0]: %s ;;", &input[0] );
  if (!strcmp(&input[0] , "\n")) { // ! is providing this is true
    curr_command->new_ln = true;
    return curr_command;
    printf("rainbow "); 
  }

	// Tokenize the input
	char *token = strtok(input, " \n");
	while(token){
		if ( !strcmp(token,"<") ) {
			curr_command->input_file = strdup(strtok(NULL," \n"));
		} else if (!strcmp(token,">")) {
			curr_command->output_file = strdup(strtok(NULL," \n"));
		} else if (!strcmp(token,"&")) {
			curr_command->is_bg = true;
		} 
    // else if(strcmp(token,"\n")){
		// 	printf("rainbow;)");
		// } 
    else {
			curr_command->argv[curr_command->argc++] = strdup(token);
		}
		token=strtok(NULL," \n");
	}
	return curr_command;
}

/*
  Adapted from provided content

  MAIN Function
    Handles the prompting continuously
    conditionally parses and passes valid arguments for command processing or pass

    Tokenizes a line of character-string user-input data;
    Parses the string into appropriate structure data types;
    catches special symbols <, > and & , conditionally encodes behavior;
    Arguments: none
    Returns: int success / failure

  // use and adaption from personal suhrm_assignment2.c, suhrm_assignment3.c, CS374 course instructional material and code
*/
int main()
{
	struct command_line *curr_command;
  int childStatus; 
  char* newLine = "\n";
  
	while(true)
	{
		curr_command = parse_input();
    // need to parse what the input is here
      // if (blank line) (# comment with pound) { skip to next re-prompt : } // PASS
      // else if (parsed input) { fork and pass to child to execv } // PROCESS

    // printf("%s",curr_command->argv[0]);

    // char* first = curr_command->argv[0];
    // if ( !strcmp(curr_command->argv[0], newLine)) { 
    //   printf("Newly WED ;*) \n");
    
    if ( (curr_command->new_ln)) { 
      printf("Newly WED ;*) \n");
    } else {

      char *newargv[] = { curr_command->argv[0], curr_command->argv[1], NULL };

      pid_t firstChild = fork();

      if (firstChild == -1) {
        perror("fork() failed!");
        exit(EXIT_FAILURE);
      } else if(firstChild == 0) {
        // char *newargv[] = { "/bin/ls", "-al", NULL }; // works instantly when passed as constant 
        execv(newargv[0], newargv); // kinda works? when prompt is passed "/bin/ls -al" returns error (execv: Bad address), tho still prints ls of directory
        // Now use execv to run "hello_world"
        //execv("hello_world", newargv);
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
    }
	}
	return EXIT_SUCCESS;
}
