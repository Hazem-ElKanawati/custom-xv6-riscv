#include "user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: sleep <ticks>\n");
    exit(1);
  }

  // Detect negative values BEFORE atoi()
  if (argv[1][0] == '-') {
    printf("sleep: ticks must be >= 0\n");
    exit(1);
  }

  int ticks = atoi(argv[1]);

  sleep(ticks);
  exit(0);
}
