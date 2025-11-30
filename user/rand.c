// user/rand.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  printf("randoms: ");
  for(i = 0; i < 10; i++){
    int r = rand();
    printf("%d ", r);
  }
  printf("\n");
  exit(0);
}
