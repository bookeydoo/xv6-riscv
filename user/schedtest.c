
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pid;
  int k, nprocess = 10;
  int z, steps = 1000000;
  uint total_turnaround = 0;
  uint total_waiting = 0;

  printf("Starting schedtest with %d processes\n", nprocess);

  for (k = 0; k < nprocess; k++) {
    sleep(2);  // Ensure different creation times

    pid = fork();
    if (pid < 0) {
      printf("fork failed!\n");
      exit(1);
    }
    else if (pid == 0) {
      // Child - allocate buffers on heap instead of stack
      printf("[pid=%d] created\n", getpid());

      // Allocate 2KB on heap using sbrk
      char *buffer_src = sbrk(1024);
      char *buffer_dst = sbrk(1024);

      if(buffer_src == (char*)-1 || buffer_dst == (char*)-1) {
        printf("[pid=%d] sbrk failed\n", getpid());
        exit(1);
      }

      // Initialize buffers
      for(int i = 0; i < 1024; i++) {
        buffer_src[i] = i % 256;
      }

      // Do the memmove operations
      for (z = 0; z < steps; z++) {
        memmove(buffer_dst, buffer_src, 1024);
        memmove(buffer_src, buffer_dst, 1024);
      }

      exit(0);
    }
  }

  // Parent collects metrics
  printf("Parent: collecting metrics\n");
  for (k = 0; k < nprocess; k++) {
    int retime, rutime, stime;
    pid = GetMetrics(&retime, &rutime, &stime);

    if(pid > 0){
      uint tat = retime + rutime;
      uint wt = retime;
      total_turnaround += tat;
      total_waiting += wt;
      printf("[pid=%d] terminated | TAT=%d | WT=%d\n", pid, tat, wt);
    } else {
      printf("GetMetrics failed\n");
    }
  }

  printf("Average Turnaround Time: %d ticks\n", total_turnaround / nprocess);
  printf("Average Waiting Time: %d ticks\n", total_waiting / nprocess);

  exit(0);
}
