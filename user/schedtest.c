#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int main(int argc, char *argv[]) {

  int pid;
  int k, nprocess = 10;
  int z, steps = 1000000;
  char buffer_src[1024], buffer_dst[1024];

  uint total_turnaround=0;
  uint total_waiting=0;


  for (k = 0; k < nprocess; k++) {
    // ensure different creation times (proc->ctime)
    // needed for properly testing FCFS scheduling
    sleep(2);

    pid = fork();
    if (pid < 0) {
      printf("%d failed in fork!\n", getpid());
      exit(0);

    }
    else if (pid == 0) {
      // child
      printf("[pid=%d] created\n", getpid());

      for (z = 0; z < steps; z += 1) {
         // copy buffers one inside the other and back
         // used for wasting cpu time
         memmove(buffer_dst, buffer_src, 1024);
         memmove(buffer_src, buffer_dst, 1024);
      }
      exit(0);
    }
  }

  for (k = 0; k < nprocess; k++) {
    int retime,rutime,stime;
    pid=GetMetrics(&retime,&rutime,&stime);
    if(pid>0){
      uint Tat=retime+rutime+stime;
      uint wt= retime;
      total_turnaround += Tat;
      total_waiting    += wt;

      printf("[pid=%d] terminated | TAT=%d | WT=%d\n", pid, Tat, wt);
    }
  }
   printf("Average Turnaround Time: %.2f ticks\n", (float)total_turnaround / nprocess);
   printf("Average Waiting Time: %.2f ticks\n", (float)total_waiting / nprocess);

  exit(0);
}
