#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../lib/commandlinereader.h"

//run inputs/random-x32-y32-z3-n64.txt
//run inputs/random-x256-y256-z5-n256.txt

int global_run = 1;
int global_MAXCHILDREN = 4;

int main(int argc, char **argv){
  int pid, *childsPid, childCount = 0;
  char command[256], *args[3];
  int argsSiz = 3, commandSiz = 256;

  if(argv[1]){
    //Has global_MAXCHILDREN as argument
    global_MAXCHILDREN = atoi(argv[1]);
  }
  childsPid = (int *)malloc((sizeof(int) * global_MAXCHILDREN) + (100*sizeof(int)));

  while(global_run){
    readLineArguments(args, argsSiz, command, commandSiz);

    if(!strcmp(args[0], "run")){
        //child
        if(childCount < global_MAXCHILDREN){
          if((pid = fork()) == 0){
            childCount++;
            childsPid[childCount] = getpid();
            execl("./CircuitRouter-SeqSolver", "CircuitRouter-SeqSolver", args[1], (char*)NULL);
            childCount--;
            exit(0);
          }
          else if(pid == -1){
            //error
            printf("Could not create a process, sorry\n");
          }
          else{
            //parent
            if(childCount == global_MAXCHILDREN){
              printf("parent awaiting\n");
              wait(NULL);
            }
          }
        }
        else if(childCount == global_MAXCHILDREN){
          printf("max reached, waiting\n");
          wait(NULL);
          if((pid = fork()) == 0){
            printf("wait passed\n");
            childCount++;
            childsPid[childCount] = getpid();
            execl("./CircuitRouter-SeqSolver", "CircuitRouter-SeqSolver", args[1], (char*)NULL);
            childCount--;
            exit(0);
          }
          else if(pid == -1){
            //error
            printf("Could not create a process, sorry\n");
          }
          else{
            //parent
            if(childCount == global_MAXCHILDREN){
              printf("parent awaiting\n");
              wait(NULL);
            }
          }
        }
      }
    else if(!strcmp(args[0], "exit")){

      return(0);
    }
    else{
      printf("Invalid command\n");
    }
  }
  return 0;
}
