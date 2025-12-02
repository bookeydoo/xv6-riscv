#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_LINE 512

// Reads a line from fd into buf; returns length or 0 on EOF
int readline(int fd, char *buf) {
    int i = 0;
    char c;
    while (1) {
        int n = read(fd, &c, 1);
        if (n < 1)  // EOF
            break;
        if (c == '\n') {
            buf[i] = 0;
            return i + 1; // include newline
        }
        buf[i++] = c;
        if (i >= MAX_LINE - 1) { // too long but still handle
            buf[i] = 0;
            return i;
        }
    }
    if (i == 0) return 0;
    buf[i] = 0;
    return i;
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "Usage: diff <file1> <file2>\n");
        exit(1);
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        fprintf(2, "diff: cannot open %s\n", argv[1]);
        exit(1);
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 < 0) {
        fprintf(2, "diff: cannot open %s\n", argv[2]);
        close(fd1);
        exit(1);
    }

    char line1[MAX_LINE];
    char line2[MAX_LINE];

    int lineno = 1;
    int diff_found = 0;

    while (1) {
        int n1 = readline(fd1, line1);
        int n2 = readline(fd2, line2);

        if (n1 == 0 && n2 == 0)
            break;

        if (n1 > 0 && n2 > 0) {
            // Both have a line
            if (strcmp(line1, line2) != 0) {
                diff_found = 1;
                printf("Line %d differs:\n", lineno);
                printf("< %s\n", line1);
                printf("> %s\n", line2);
            }
        } else if (n1 > 0 && n2 == 0) {
            diff_found = 1;
            printf("Line %d only in %s:\n", lineno, argv[1]);
            printf("< %s\n", line1);
        } else if (n1 == 0 && n2 > 0) {
            diff_found = 1;
            printf("Line %d only in %s:\n", lineno, argv[2]);
            printf("> %s\n", line2);
        }

        lineno++;
    }

    if (!diff_found)
        printf("Files are identical\n");

    close(fd1);
    close(fd2);
    exit(0);
}
