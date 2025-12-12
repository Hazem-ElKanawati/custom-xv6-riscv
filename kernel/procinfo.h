#ifndef _PINFO_H_
#define _PINFO_H_

#include "types.h"

struct pinfo {
  int pid;
  int ppid;
  int state;
  char name[16];
  uint64 memsize;
  uint64 creation_time;
  uint64 run_time;
  uint64 waiting_time;
  uint end_time;
};

#endif
