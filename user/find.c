#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char* fmtname(char *path)
{
  static char buff[DIRSIZ+1];
  char *Path;

  // Find first character after last slash.
  for(Path=path+strlen(path); Path >= path && *Path != '/'; Path--)
    ;
  Path++;

  // Return blank-padded name.
  if(strlen(Path) >= DIRSIZ)
    return Path;
  memmove(buff, Path, strlen(Path));
  memset(buff+strlen(Path), ' ', DIRSIZ-strlen(Path));
  return buff;
}


void strcat_inplace(char *s, const char *t);

void find(char* Query,char* path){
  char buff[512],*Name;
  int fd ; //file descripitor
  struct dirent directEntry;
  struct stat Stat;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &Stat) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(Stat.type == T_DEVICE || Stat.type == T_FILE){
    char NameBuf[DIRSIZ+1];
    memmove(NameBuf,fmtname(path),DIRSIZ);
    NameBuf[DIRSIZ]=0;

    if (strcmp(NameBuf,Query)==0){
      printf("File found at :%s",path);
    }
    close(fd);
    return;
  }

  if(Stat.type == T_DIR){
    if( strlen(path) + 1 +DIRSIZ+1 > sizeof buff ){
      printf("Path is too long\n");
      close(fd);
      return ;
    }

    strcpy(buff,path);
    Name=buff+strlen(buff);
    *Name++ ='/';

    while(read(fd, &directEntry, sizeof(directEntry)) == sizeof(directEntry)){


      if(directEntry.inum == 0)
        continue;
      //Assign ChildDir value using a new Buffer that is nullterminated
      char ChildDirBuf[DIRSIZ+1];
      memmove(ChildDirBuf,directEntry.name,DIRSIZ);
      ChildDirBuf[DIRSIZ]=0;


      if(!strcmp(ChildDirBuf,".") || !strcmp(ChildDirBuf,".."))
        continue;


      char NewPath[512];
      strcpy(NewPath,buff);

      if(strlen(buff)+ strlen(ChildDirBuf) >= sizeof(NewPath)){
        printf("Path too long\n");
        continue;
      }

      strcat_inplace(NewPath,ChildDirBuf);

      find(Query,NewPath);

    }
  }
  printf("failed to find the file\n");
  close(fd);
}

int main(int argc ,char* argv[]){

  if (argc < 3 ){
    printf("Invalid Usage: You need to pass in the file you are looking for and which directory \n");
    printf("An example of how to use is : find .\n");
    exit(0);
  }

  find(argv[1],argv[2]);

  return 0;
}
