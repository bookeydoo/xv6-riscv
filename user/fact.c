
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int factorial(int n){
    if (n==0 || n==1)
      return 1;

    return n*factorial(n-1);
}

int main(int  argc,char* argv[])
{
  if (argc < 2){
    printf("Invalid Usage: You need to pass in a positive value to factorize\n");
    exit(1);
  }

  if (strcmp(argv[1],"?") == 0){ //decimal value for ?
    printf("Factorial function takes a single positive value \n");
    exit(1);
  }

  if (strcmp(argv[1],"-") == 0){ //decimal value for ?
    printf("Factorial function takes a single positive value \n");
    exit(1);
  }

  int n=atoi(argv[1]);

  printf("%d\n",factorial(n));


  return 0;
}
