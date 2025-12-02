#include "types.h"
#include "riscv.h"
#include "defs.h"

extern uint64 syscall_count;

uint64
sys_countsyscall(void)
{
    return syscall_count;
}
