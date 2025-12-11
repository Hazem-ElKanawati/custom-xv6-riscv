#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SCHED_PRIORITY 3 // Ensure this matches your kernel
#define NUM_PROCS 12     // 4 High, 4 Med, 4 Low

void worker(int prio) {
  setpriority(getpid(), prio);
  for (volatile int i = 0; i < 100000000; i++);
  exit(0);
}

int main() {
  printf("Starting Priority Load Test on 3 CPUs...\n");
  setsched(SCHED_PRIORITY);

  // Launch 4 High Priority (10) - They will grab the 3 CPUs first
  for(int i=0; i<4; i++) {
    if(fork() == 0) worker(10);
  }

  // Launch 4 Medium Priority (5) - They will wait for High to finish
  for(int i=0; i<4; i++) {
    if(fork() == 0) worker(5);
  }

  // Launch 4 Low Priority (1) - They will wait for EVERYONE
  for(int i=0; i<4; i++) {
    if(fork() == 0) worker(1);
  }

  // Wait for all 12
  for(int i=0; i<12; i++) wait(0);

  printf("Priority Test Finished.\n");
  print_stats();
  exit(0);
}
