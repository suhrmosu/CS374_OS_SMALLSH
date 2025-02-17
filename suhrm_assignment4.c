#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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
