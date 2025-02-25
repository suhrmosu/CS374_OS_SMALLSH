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
  bool exit;
  bool stat;
  bool change_wd;
  bool re_in;
  bool re_ot;
};

struct command_line *parse_input();

  
/*
  Adapted from provided content

  Function parse_input
    Tokenizes a line of character-string user-input data;
    Parses the string into appropriate structure data types;
    catches special symbols <, > and & , conditionally encodes behavior;
    command syntax as follows, requested from user in-function
      command [arg1 arg2 ...] [< input_file] [> output_file] [&]
    Arguments: none
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
  } else if (!strncmp(&input[0] , "#", 1)) {
    curr_command->new_ln = true;
    return curr_command;
  } 
  // else if (!strncmp(&input[0] , "cd",2)) {
  //   // printf("change p");
  //   curr_command->change_wd = true;
  //   // curr_command->new_ln = true;
  //   // return curr_command;
  // }

	// Tokenize the input
	char *token = strtok(input, " \n");
	while(token){
		if ( !strcmp(token,"<") ) {
      curr_command->re_in = true;
			curr_command->input_file = strdup(strtok(NULL," \n"));
		} else if (!strcmp(token,">")) {
      curr_command->re_ot = true;
			curr_command->output_file = strdup(strtok(NULL," \n"));
		} else if (!strcmp(token,"&")) {
			curr_command->is_bg = true;
		} else if (!strcmp(token,"exit")) {
			curr_command->exit = true;
		} else if (!strcmp(token,"status")) {
			curr_command->stat = true;
		} else if (!strcmp(token,"cd")) {
      curr_command->change_wd = true;
		} 
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
  bool active_sh = true;
  int exit_stat = 0;
  // int arrBg[25];
  // int i;
  pid_t waitChild = 44444; // need to make this a dynamic array ... 

	while(active_sh)
	{
    pid_t childPid = waitpid(waitChild, &childStatus, WNOHANG);
    // status -1 for error/ no child
    // status 0 for child still running
    // status = PID for child completed process
    if (childPid == waitChild) {
      // need to update this to conditionally format the output... catch "termination by signal on pid"
      printf("background pid %d is done: exit value %d \n", waitChild, WEXITSTATUS(childStatus));
    }
    printf("background pid %d is done: terminated by signal %d waitpid return %d \n", waitChild, WEXITSTATUS(childStatus), childPid);

		curr_command = parse_input();
    // need to parse what the input is here
      // if (blank line) (# comment with pound) { skip to next re-prompt : } // PASS
      // else if (parsed input) { fork and pass to child to execv } // PROCESS
    
    if ( (curr_command->new_ln) ) { 
      // pass to new line on { \n } or { # }
      // printf("New line ;*) \n");
    } else if ( (curr_command->exit) ) { 
      // exit loop
      active_sh = false;
    } else if ( (curr_command->stat) ) { 
      // display status last command (or 0 if none yet)
      printf("exit value %d \n", exit_stat);
    } else if ( (curr_command->change_wd) ) { 
      // change working directory, set as PWD
      if (curr_command->argv[0]) {
        setenv("PWD", curr_command->argv[0], 1);
      } else {
        setenv("PWD", getenv("HOME"), 1);
      }
      chdir(getenv("PWD")); 
    } else {
      // Executing Other Commands:
      // utilizing 3 built-in command by using fork(), exec() and waitpid()

      // fork off child process to call exec()
      pid_t firstChild = fork();

      if (firstChild == -1) {
        perror("fork() failed!");
        exit(EXIT_FAILURE);
      } else if (firstChild == 0) {
        // This is the child fork process

        // redirect I/O
        if (curr_command->re_in) {
          // set input file from struct
          int sourceFD = open(curr_command->input_file, O_RDONLY);
          if (sourceFD == -1) { 
            perror("source open()"); 
            exit(1); 
          }
          fcntl(sourceFD, F_SETFD, FD_CLOEXEC);

          // Write the file descriptor to stdout
          // printf("File descriptor of input file = %d\n", sourceFD); 

          // Redirect stdin to source file
          int result = dup2(sourceFD, 0);
          if (result == -1) { 
            perror("source dup2()"); 
            exit(2); 
          }
        }
        // if not re_in && is_bg --> { standard input must be redirected to /dev/null. }
        else if ((!curr_command->re_in) && (curr_command->is_bg)) {
          int sourceFD = open("/dev/null", O_RDONLY);
          if (sourceFD == -1) { 
            perror("source open()"); 
            exit(1); 
          }
          fcntl(sourceFD, F_SETFD, FD_CLOEXEC);
          // Redirect stdin to /dev/null
          int result = dup2(sourceFD, 0);
          if (result == -1) { 
            perror("input /dev/null dup2()"); 
            exit(2); 
          }
        }
        if (curr_command->re_ot) {
          // set output file from struct
          int targetFD = open(curr_command->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
          if (targetFD == -1) { 
            perror("target open()"); 
            exit(1); 
          }

          fcntl(targetFD, F_SETFD, FD_CLOEXEC);

          // Write the file descriptor to stdout
          // printf("File descriptor of output file = %d\n", targetFD);
          
          // Redirect stdout to target file
          int result = dup2(targetFD, 1);
          if (result == -1) { 
            perror("target dup2()"); 
            exit(2); 
          }
        }
        // if not re_ot && is_bg --> { standard output must be redirected to /dev/null. }
        else if ((!curr_command->re_ot) && (curr_command->is_bg)) {
          int targetFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
          if (targetFD == -1) { 
            perror("target open()"); 
            exit(1); 
          }
          fcntl(targetFD, F_SETFD, FD_CLOEXEC);
          // Redirect stdout to target file
          int result = dup2(targetFD, 1);
          if (result == -1) { 
            perror("target dup2()"); 
            exit(2); 
          }
        }
        // if (curr_command->is_bg) {
        //   // store pid of background children 
        //   // store(getpid());
          
        //   // int bg_child = getpid();
        //   waitChild = getpid();
        //   // int length = sizeof(arrBg) / sizeof(arrBg[0]);
        //   // printf(" child pid %d length arrBg %d", bg_child, length);
        //   // arrBg[length] = bg_child;
        // }

        // utilizing the p argument will search path for file input as first argument
        execvp(curr_command->argv[0],  curr_command->argv); 

        /* execve() returns only on error */
        perror(curr_command->argv[0]);
        exit(EXIT_FAILURE);
      } else {
        // this is to wait for the fork first child to finish/ return, passing the firstChild PID value

        // if foreground process (NOT background), wait for complete
        if (!curr_command->is_bg) {
          // printf("Child's pid = %d\n", firstChild);

          firstChild = waitpid(firstChild, &childStatus, 0);

          // set status from finished FOREGROUND command
          exit_stat =  WEXITSTATUS(childStatus);

          // printf("waitpid returned value %d\n", firstChild);
          // if(WIFEXITED(childStatus)){
          //   printf("Child %d exited normally with status %d\n", firstChild, WEXITSTATUS(childStatus));
          // } else{
          //   printf("Child %d exited abnormally due to signal %d\n", firstChild, WTERMSIG(childStatus));
          // }
          //
        }
        // else if background process, proceed. 
        else if (curr_command->is_bg) {
          printf("background pid is %d \n", firstChild);
          // pid_t childPid = waitpid(firstChild, &childStatus, 0);
          // printf("background pid %d is done: terminated by signal %d \n", childPid, WEXITSTATUS(childStatus));
          waitChild = firstChild;
        }
      }
    }
	}
	return EXIT_SUCCESS;
}
