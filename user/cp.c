#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[512];
    int src_fd, dst_fd, n;

    if(argc != 3){
        fprintf(2, "Usage: cp <source> <destination>\n");
        exit(1);
    }

    src_fd = open(argv[1], O_RDONLY);
    if(src_fd < 0){
        fprintf(2, "cp: cannot open %s\n", argv[1]);
        exit(1);
    }

    dst_fd = open(argv[2], O_CREATE | O_WRONLY);
    if(dst_fd < 0){
        fprintf(2, "cp: cannot create %s\n", argv[2]);
        close(src_fd);
        exit(1);
    }

    while((n = read(src_fd, buf, sizeof(buf))) > 0){
        if(write(dst_fd, buf, n) != n){
            fprintf(2, "cp: write error\n");
            close(src_fd);
            close(dst_fd);
            exit(1);
        }
    }

    close(src_fd);
    close(dst_fd);
    exit(0);
}
