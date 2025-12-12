#include "user.h"

int
main(int argc, char *argv[])
{
  struct pinfo buf[64];
  int n = 64;

  int ok = getptable(n, buf);
  if(ok == 0){
    printf("getptable failed\n");
    exit(1);
  }

  printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");

  for(int i = 0; i < n; i++){
    if(buf[i].pid == 0)
      continue;

    printf("%d\t%d\t%d\t%ld\t%s\n",
           buf[i].pid,
           buf[i].ppid,
           buf[i].state,
           buf[i].memsize,
           buf[i].name);
  }

  exit(0);
}
