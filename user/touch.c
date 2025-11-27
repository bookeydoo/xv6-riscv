#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int main(int  argc,char* argv[])
{
  if (argc < 2){
    printf("Invalid Usage: You need to pass name of the file and atleast one argument\n");
    exit(1);
  }

  int fd=open(argv[1],O_CREATE | O_RDWR);
  if (fd < 0){
    printf("Touch: Failed for some reason\n");
    exit(1);
  }else{
    printf("Created file succesfully\n");
  }


  close(fd);
  return 0;
}
