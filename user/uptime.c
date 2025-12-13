#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int t = uptime();
  printf("ticks: %d (approx %d seconds)\n", t, t/100);
  return 0;
}
