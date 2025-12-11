#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXLINE 512

// Correct xv6-safe readline
// Returns: 1 = line read, 0 = EOF
int readline(int fd, char *buf) {
  int i = 0;
  char c;

  while (1) {
    int n = read(fd, &c, 1);
    if (n < 1) {
      if (i == 0) return 0;   // true EOF
      break;                  // return unfinished line
    }

    if (c == '\n')
      break;

    if (i < MAXLINE - 1)
      buf[i++] = c;
  }

  buf[i] = 0;
  return 1;
}

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("Usage: diff file1 file2\n");
    exit(1);
  }

  int fd1 = open(argv[1], 0);
  int fd2 = open(argv[2], 0);

  if (fd1 < 0 || fd2 < 0) {
    printf("diff: cannot open file(s)\n");
    exit(1);
  }

  char buf1[MAXLINE], buf2[MAXLINE];
  int line = 1;
  int has1, has2;
  int identical = 1;

  while (1) {
    has1 = readline(fd1, buf1);
    has2 = readline(fd2, buf2);

    if (!has1 && !has2)
      break;

    if (has1 && has2) {
      if (strcmp(buf1, buf2) != 0) {
        identical = 0;
        printf("Line %d differs:\n", line);
        printf("< %s\n", buf1);
        printf("> %s\n", buf2);
      }
    } else if (has1) {
      identical = 0;
      printf("Line %d only in %s:\n", line, argv[1]);
      printf("< %s\n", buf1);
    } else if (has2) {
      identical = 0;
      printf("Line %d only in %s:\n", line, argv[2]);
      printf("> %s\n", buf2);
    }

    line++;
  }

  if (identical) {
    printf("Files are identical\n");
  }

  close(fd1);
  close(fd2);
  exit(0);
}
