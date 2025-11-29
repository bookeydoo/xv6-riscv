#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"


//inital idea but unsure it would work correctly or needs malloc and i think doesn't work on xv6
//get size and malloc a buffer of size =filesize-N or just preallocate a big enough buffer but may not have the read done properly
void tail(char* filename, int N) {

  struct stat Stat;
  char Buf[512];
  int ReadBytes, Offset = 0;
  int NoOfLines = 0;
  int PrintFlag = 0;

  int fd = open(filename, 0);
  if(fd < 0){
    printf("tail: cannot open file\n");
    return;
  }

  if(fstat(fd, &Stat) < 0){
    printf("tail: cannot stat file\n");
    close(fd);
    return;
  }

  int fileSize = Stat.size;

  // if file fits entirely, print all
  if (fileSize <= N){
    while((ReadBytes = read(fd, Buf, sizeof(Buf))) > 0){
      write(1, Buf, ReadBytes);
    }
    close(fd);
    return;
  }

  int StartPoint = fileSize - sizeof(Buf);
  if(StartPoint < 0)
    StartPoint = 0;

  while (StartPoint >= 0) {

    close(fd);
    fd = open(filename, 0);

    // skip to StartPoint
    int skip = StartPoint;
    while(skip > 0) {
      int chunk = skip > sizeof(Buf) ? sizeof(Buf) : skip;
      read(fd, Buf, chunk);
      skip -= chunk;
    }

    // read chunk
    ReadBytes = read(fd, Buf, sizeof(Buf));

    // scan backwards inside this chunk
    for(int i = ReadBytes - 1; i >= 0; i--) {
      if(Buf[i] == '\n') {
        NoOfLines++;
        if(NoOfLines == N + 1) {
          Offset = StartPoint + i + 1;
          PrintFlag = 1;
          break;
        }
      }
    }

    if(PrintFlag)
        break;

    StartPoint -= sizeof(Buf);
  }

  // print output from Offset
  close(fd);
  fd = open(filename, 0);

  // skip Offset bytes
  int skip2 = Offset;
  while(skip2 > 0) {
    int chunk = skip2 > sizeof(Buf) ? sizeof(Buf) : skip2;
    read(fd, Buf, chunk);
    skip2 -= chunk;
  }

  while((ReadBytes = read(fd, Buf, sizeof(Buf))) > 0)
    write(1, Buf, ReadBytes);

  close(fd);
}

int main(int  argc,char* argv[])
{


  if (argc < 2){
    printf("Invalid Usage: Need to pass atleast a file\n");
    exit(1);
  }


  if (argc == 2){
    printf("Running default tail\n");
    tail(argv[1],5);
    exit(1);
  }


  else{
    tail(argv[1],atoi(argv[2]));
  }


  return 0;
}
