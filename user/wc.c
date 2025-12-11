#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

struct counts {
  int lines;
  int words;
  int chars;
  int longest;
};

void wc(int fd, char *name, struct counts *out)
{
  int i, n;
  int l = 0, w = 0, c = 0, inword = 0;
  int longest = 0, current = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      c++;

      if (buf[i] == '\n') {
        l++;
        if (current > longest)
          longest = current;
        current = 0;
      } else {
        current++;
      }

      if (strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if (!inword) {
        w++;
        inword = 1;
      }
    }
  }

  if (n < 0) {
    printf("wc: read error\n");
    exit(1);
  }

  // If file doesn't end with newline, update longest line
  if (current > longest)
    longest = current;

  out->lines = l;
  out->words = w;
  out->chars = c;
  out->longest = longest;

  if (fd != 0)
    close(fd);
}

void print_counts(struct counts c, int show_l, int show_w, int show_c, int show_L, char *name)
{
  if (show_l) printf("%d ", c.lines);
  if (show_w) printf("%d ", c.words);
  if (show_c) printf("%d ", c.chars);
  if (show_L) printf("%d ", c.longest);

  printf("%s\n", name);
}

int
main(int argc, char *argv[])
{
  int i;
  int show_l = 0, show_w = 0, show_c = 0, show_L = 0;

  // Parse flags first
  int arg_i = 1;
  for (; arg_i < argc && argv[arg_i][0] == '-'; arg_i++) {
    char *p = argv[arg_i] + 1;
    while (*p) {
      if (*p == 'l') show_l = 1;
      else if (*p == 'w') show_w = 1;
      else if (*p == 'c') show_c = 1;
      else if (*p == 'L') show_L = 1;
      else {
        printf("wc: unknown option %c\n", *p);
        exit(1);
      }
      p++;
    }
  }

  // If no flags, default to l w c
  if (!show_l && !show_w && !show_c && !show_L) {
    show_l = show_w = show_c = 1;
  }

  // No files → read from stdin
  if (arg_i == argc) {
    struct counts c;
    wc(0, "", &c);
    print_counts(c, show_l, show_w, show_c, show_L, "");
    exit(0);
  }

  struct counts total = {0};
  int multiple = (argc - arg_i > 1);

  // Process files
  for (i = arg_i; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY);
    if (fd < 0) {
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }

    struct counts c;
    wc(fd, argv[i], &c);

    if (multiple) {
      total.lines += c.lines;
      total.words += c.words;
      total.chars += c.chars;
      if (c.longest > total.longest)
        total.longest = c.longest;
    }

    print_counts(c, show_l, show_w, show_c, show_L, argv[i]);
  }

  // Print total if multiple files
  if (multiple) {
    print_counts(total, show_l, show_w, show_c, show_L, "total");
  }

  exit(0);
}
