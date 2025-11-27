
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"


//inital idea but unsure it would work correctly or needs malloc and i think doesn't work on xv6
//get size and malloc a buffer of size =filesize-N or just preallocate a big enough buffer but may not have the read done properly
void tail(int fd,int N){

  struct stat Stat;
  char Buf[512];
  int ReadBytes;

  //char nullchr='\0';

  if(fstat(fd,&Stat) < 0){
    printf("tail: sth went wrong with tail\n");
    exit(1);
  }

  int fileSize=Stat.size;

  int SkippedBytes=fileSize-N;

  if (fileSize <= N){
    while((ReadBytes=read(fd,Buf,512)) >0){
      write(1,Buf,ReadBytes);
    }

    return ;
  }

  int iter=0;

  while(iter<SkippedBytes){

    int toRead = SkippedBytes-iter;

    if(toRead>sizeof(Buf)){
      toRead=sizeof(Buf);
    }

    ReadBytes=read(fd,Buf,toRead);

    if(ReadBytes <= 0)
       break;

    iter+=ReadBytes;
  }


  while((ReadBytes=read(fd,Buf,sizeof(Buf)))>0){
      write(1,Buf,ReadBytes);
  }



  return ;

}

int main(int  argc,char* argv[])
{

  int fd=0;

  if (argc < 2){
    printf("Invalid Usage: Need to pass atleast a file\n");
    exit(1);
  }

  if( (fd=open(argv[1],O_RDONLY) )< 0){
    printf("tail:Couldn't Open file %s\n",argv[1]);
    exit(1);
  }

  if (argc == 2){
    printf("Running default tail\n");
    tail(fd,5);
    exit(1);
  }


  else{
    tail(fd,atoi(argv[2]));
  }


  close(fd);
  return 0;
}
