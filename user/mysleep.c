#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[]){

  if(argc == 2 && strcmp(argv[1],"?") == 0){
      printf("Usage: Sleep n \n ");
      return 0;
  }
  if(argc > 2){
      printf("Invalid Usage, you need to input only one number \n ");
      return 0;
  }

  if(argc < 2){
      printf("You need to pass atleast one number");
      return 0;
  }

  int sleepTime=atoi(argv[1]);

  printf("Gonna sleep for %d ticks\n",sleepTime);
  sleep(sleepTime);
  printf("Woke up !!\n");

  return 0;
}
