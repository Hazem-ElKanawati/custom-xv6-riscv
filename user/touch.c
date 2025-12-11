#include "user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: touch <filename>\n");
    exit(1);
  }
  //program requires exactly 1 argument, the file name.

  char *filename = argv[1];
  //argv[1] is the string the user typed after touch

  // Try to create file using mknod.
  // If the file exists, mknod returns -1.
  if (mknod(filename, 0, 0) < 0) {
    printf("touch: file '%s' already exists or cannot be created\n", filename);
    exit(1);
  }

  exit(0);
}
