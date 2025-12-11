#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "Usage: mv <source> <destination>\n");
        exit(1);
    }

    char *src = argv[1];
    char *dst = argv[2];

    if(link(src, dst) < 0){
        fprintf(2, "mv: cannot link %s to %s\n", src, dst);
        exit(1);
    }

    if(unlink(src) < 0){
        fprintf(2, "mv: cannot unlink %s\n", src);
        unlink(dst);
        exit(1);
    }

    exit(0);
}
