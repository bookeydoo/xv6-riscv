#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct rtcdate r;
  if (datetime(&r) < 0) {
    printf("datetime: syscall failed\n");
    return 1;
  }
  printf("%d-", r.year);
  if (r.month < 10) printf("0");
  printf("%d-", r.month);
  if (r.day < 10) printf("0");
  printf("%d ", r.day);
  if (r.hour < 10) printf("0");
  printf("%d:", r.hour);
  if (r.minute < 10) printf("0");
  printf("%d:", r.minute);
  if (r.second < 10) printf("0");
  printf("%d\n", r.second);
  return 0;
}
