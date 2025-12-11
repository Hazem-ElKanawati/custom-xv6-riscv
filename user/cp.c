#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define BUF_SIZE 512

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("Usage: cp source dest\n");
    exit(1);
  }

  char *src = argv[1];
  char *dst = argv[2];

  int fdsrc = open(src, 0);                // read-only
  if (fdsrc < 0) {
    printf("cp: cannot open %s\n", src);
    exit(1);
  }

  // Create/truncate destination. Use O_CREATE | O_WRONLY so file is created if missing.
  int fddst = open(dst, O_CREATE | O_WRONLY);
  if (fddst < 0) {
    printf("cp: cannot create %s\n", dst);
    close(fdsrc);
    exit(1);
  }

  char buf[BUF_SIZE];
  int n;
  while ((n = read(fdsrc, buf, BUF_SIZE)) > 0) {
    int written = 0;
    // ensure full write
    while (written < n) {
      int w = write(fddst, buf + written, n - written);
      if (w < 0) {
        printf("cp: write error\n");
        close(fdsrc);
        close(fddst);
        exit(1);
      }
      written += w;
    }
  }
  if (n < 0) {
    printf("cp: read error\n");
    // continue to close files and exit with error
    close(fdsrc);
    close(fddst);
    exit(1);
  }

  close(fdsrc);
  close(fddst);
  exit(0);
}
