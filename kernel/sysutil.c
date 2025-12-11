#include "types.h"
#include "riscv.h"
#include "defs.h"

extern uint64 syscall_count;
extern int kbd_intr_count;

uint64
sys_countsyscall(void)
{
    return syscall_count;
}


uint64
sys_kbdint(void)
{
  return kbd_intr_count;
}
