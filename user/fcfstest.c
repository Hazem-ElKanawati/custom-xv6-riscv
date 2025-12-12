#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Ensure these match your kernel/param.h definitions
#define SCHED_FCFS 2

void
heavy_task(int id)
{
  int x = 0;
  // Heavy computation to ensure it occupies the CPU
  for(volatile int i = 0; i < 100000000; i++){
    x += i;
  }
  printf("  [Task %d] Finished computation.\n", id);
  exit(0);
}

int
main(int argc, char *argv[])
{
  printf("Starting FCFS Scheduler Test...\n");

  // 1. Switch to FCFS
  if (setsched(SCHED_FCFS) < 0) {
    printf("Error: setsched failed.\n");
    exit(1);
  }
  printf("Scheduler set to FCFS mode.\n");

  // 2. Fork 3 processes sequentially
  // In FCFS, Task 1 MUST finish before Task 2 starts, etc.

  int pid1 = fork();
  if(pid1 == 0){
    heavy_task(1);
  }
  sleep(1); // Small sleep to ensure Creation Times are distinct

  int pid2 = fork();
  if(pid2 == 0){
    heavy_task(2);
  }
  sleep(1);

  int pid3 = fork();
  if(pid3 == 0){
    heavy_task(3);
  }

  // 3. Wait for all to finish
  wait(0);
  wait(0);
  wait(0);

  printf("FCFS Test Finished.\n");
  print_stats();
  exit(0);
}
