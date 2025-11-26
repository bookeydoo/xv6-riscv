#include "kernel/types.h"
#include "kernel/stat.h"
#include<kernel/fs.h>
#include "user/user.h"

int main(int argc ,char* argv[]){

  if (argc < 3 ){
    printf("Invalid Usage: You need to pass in the file you are looking for and which directory \n");
    printf("An example of how to use is : find ls .");
    exit(0);
  }

  char* Query=argv[1];
  char* Dir=argv[2];


}
