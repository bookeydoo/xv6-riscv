#include<kernel/types.h>
#include <user/user.h>
#include <kernel/fcntl.h>

#define MAXPROC 64

int
main(int argc, char *argv[])
{
    struct uproc buf[MAXPROC];
    int success;
    int i;

    // Call the syscall
    success = getptable(MAXPROC, (char *)buf);
    if(success == 0){
        printf("getptable failed\n");
        exit(-1);
    }

    printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");
    for(i = 0; i < MAXPROC; i++){
        // Skip unused entries
        if(buf[i].ProcessState == 0) // assuming 0 == UNUSED
            continue;

        printf("%d \t %d \t %d \t %ld \t %s\n",
               buf[i].pid,
               buf[i].ppid,
               buf[i].ProcessState,
               buf[i].size,
               buf[i].name);
    }

    exit(-1);
}
