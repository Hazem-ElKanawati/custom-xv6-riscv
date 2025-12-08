#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SCHED_FCFS 2  // Ensure this matches your kernel/param.h
#define NUM_PROCS 10  // 10 processes vs 3 CPUs = GUARANTEED WAITING

void worker(int id) {
  // Heavy work to hold the CPU
  for (volatile int i = 0; i < 100000000; i++);
  exit(0);
}

int main() {
  printf("Starting FCFS Load Test on 3 CPUs...\n");

  // 1. Set Scheduler
  if (setsched(SCHED_FCFS) < 0) {
    printf("Error: setsched failed.\n");
    exit(1);
  }

  // 2. Fork 10 processes rapidly
  for(int i = 0; i < NUM_PROCS; i++){
    if(fork() == 0){
      worker(i);
    }
  }

  // 3. Wait
  for(int i = 0; i < NUM_PROCS; i++){
    wait(0);
  }

  printf("FCFS Test Finished.\n");
  print_stats();
  exit(0);
}
