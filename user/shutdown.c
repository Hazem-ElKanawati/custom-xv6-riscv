#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  shutdown();
  // If QEMU didn't exit, print a message and exit
  printf("shutdown returned\n");
  exit(0);
}
