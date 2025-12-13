#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[]) {

  if (argc < 2){
    printf("No args passed using the default scheduler :FCFS\n");
    setsched(0);//FCFS
    exit(1);
  }
  if (strcmp(argv[1],"?") == 0){ //decimal value for ?
    printf("Pass an int from 0 to 2\n 0 is for FCFS ,1 is for RR , 2 is for PBS\n");
    exit(1);
  }

  int choice =atoi(argv[1]);

  if (choice == 0){
    printf("Selected FCFS \n");
  }
  if (choice == 1){
    printf("Selected Round Robin\n");
  }
  if (choice == 2){
    printf("Selected Priority based\n");
  }
  setsched(choice);
  exit(1);
}
