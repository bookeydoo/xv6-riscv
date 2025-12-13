#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int getptable(int nproc,char* buffer);
int getProcessMetrics(int*,int*,int*);

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}


uint64
sys_getppid(void)
{
  return myproc()->parent->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_myrand(void)
{
  uint xticks;
  uint a=166425;
  uint m=1408576;
  uint c=1013569;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);

  //equation is :Xn+1 =(a * Xn +c )mod m
  //where x is the seed(time)
  int val=(a*xticks+c) % m;

  return val;


}

uint64
sys_shutdown(void)
{
    volatile uint32 *poweroff = (uint32*)0x100000;

    *poweroff = 0x5555;   // tell QEMU to exit

    return 0;
}

int
sys_getptable(void)
{
    int n;
    uint64 buf; // user-space pointer

    argint(0, &n); // get nproc

    argaddr(1, &buf);

    return getptable(n, (char *)buf);
}

uint64
sys_setsched(void)
{
  int mode;
  argint(0, &mode);

  if (mode != SCHED_ROUND_ROBIN &&
      mode != SCHED_FCFS)
    return -1;

  sched_mode = mode;
  return 0;
}


uint64
sys_GetMetrics(void)
{
    uint64 u_retime, u_rutime, u_stime;
    int k_retime, k_rutime, k_stime;
    struct proc *p = myproc();

    argaddr(0, &u_retime) ;
    argaddr(1, &u_rutime) ;
    argaddr(2, &u_stime) ;

    int pid = getProcessMetrics(&k_retime, &k_rutime, &k_stime);
    if (pid < 0)
        return -1;

    if (copyout(p->pagetable, u_retime, (char*)&k_retime, sizeof(int)) < 0 ||
        copyout(p->pagetable, u_rutime, (char*)&k_rutime, sizeof(int)) < 0 ||
        copyout(p->pagetable, u_stime,  (char*)&k_stime,  sizeof(int)) < 0)
        return -1;

    return pid;
}
