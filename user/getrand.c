#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int r=myrand();
  printf("%d rand val 1\n",r);

  exit(0);
}
