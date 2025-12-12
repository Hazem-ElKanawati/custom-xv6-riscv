#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SCHED_PRIORITY 3 // Ensure this matches your kernel

void worker(int prio) {
  setpriority(getpid(), prio);
  for (volatile int i = 0; i < 100000000; i++);
  exit(0);
}
int main() {
  printf("Starting REVERSE Priority Test...\n");
  setsched(SCHED_PRIORITY);

  // 1. Launch LOW Priority First (Priority 1)
  // These will get LOW PIDs (e.g., 50-64)
  for(int i=0; i<15; i++) {
    if(fork() == 0) worker(1);
  }

  // 2. Launch MEDIUM Priority (Priority 5)
  for(int i=0; i<15; i++) {
    if(fork() == 0) worker(5);
  }

  // 3. Launch HIGH Priority LAST (Priority 10)
  // These will get HIGH PIDs (e.g., 80-94)
  for(int i=0; i<15; i++) {
    if(fork() == 0) worker(10);
  }

  for(int i=0; i<45; i++) wait(0);

  printf("Reverse Test Finished.\n");
  print_stats();
  exit(0);
}
