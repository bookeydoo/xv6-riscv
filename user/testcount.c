#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    printf("System calls so far: %d\n", countsyscall());

    // Generate some syscalls
    getpid();     // syscall
    sleep(1);     // syscall
    getpid();     // syscall
    countsyscall(); // syscall (calling itself also counts!)

    printf("System calls so far: %d\n", countsyscall());

    exit(0);
}
