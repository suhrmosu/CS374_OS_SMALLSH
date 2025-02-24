#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  if (argc != 3) {
    printf("Usage: ./sort_file <input filename> <output filename>\n");
    exit(1);
  }
  // Open the source file
  int sourceFD = open(argv[1], O_RDONLY);
  if (sourceFD == -1) { 
    perror("source open()"); 
    exit(1); 
  }
  fcntl(sourceFD, F_SETFD, FD_CLOEXEC);

  // Write the file descriptor to stdout
  printf("File descriptor of input file = %d\n", sourceFD); 

  // Redirect stdin to source file
  int result = dup2(sourceFD, 0);
  if (result == -1) { 
    perror("source dup2()"); 
    exit(2); 
  }

  // Open target file
  int targetFD = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (targetFD == -1) { 
    perror("target open()"); 
    exit(1); 
  }

  fcntl(targetFD, F_SETFD, FD_CLOEXEC);

  // Write the file descriptor to stdout
  printf("File descriptor of output file = %d\n", targetFD);
  
  // Redirect stdout to target file
  result = dup2(targetFD, 1);
  if (result == -1) { 
    perror("target dup2()"); 
    exit(2); 
  }
  // Run the sort program using execlp().
  // The stdin and stdout are pointing to files
  execlp("sort", "sort", NULL);
  return(0);
}