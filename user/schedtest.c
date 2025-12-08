#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Ensure these match what you defined in your kernel
#define SCHED_ROUND_ROBIN 0
#define SCHED_FCFS        1
#define SCHED_PRIORITY    2

void
cpu_bound_task(int id, int prio)
{
  int x = 0;
  // Set my own priority
  if (setpriority(getpid(), prio) < 0) {
    printf("Error setting priority for %d\n", getpid());
  }

  // Burn CPU time to force scheduling
  for (volatile int i = 0; i < 100000000; i++) {
    x += i;
  }
  printf("Process %d (Priority %d) finished.\n", id, prio);
  exit(0);
}

int
main(int argc, char *argv[])
{
  printf("Starting Scheduler Test...\n");

  // 1. Switch to Priority Scheduler
  if (setsched(SCHED_PRIORITY) < 0) {
    printf("Error: setsched failed (did you implement it?)\n");
    exit(1);
  }
  printf("Scheduler set to PRIORITY mode.\n");

  // 2. Fork 3 processes with different priorities
  // High Priority (should finish 1st)
  if (fork() == 0) {
    cpu_bound_task(1, 10);
  }

  // Medium Priority (should finish 2nd)
  if (fork() == 0) {
    cpu_bound_task(2, 5);
  }

  // Low Priority (should finish 3rd)
  if (fork() == 0) {
    cpu_bound_task(3, 1);
  }

  // 3. Wait for all children to finish
  wait(0);
  wait(0);
  wait(0);

  printf("Test finished.\n");


  // Reset to default (Round Robin) before exiting
  setsched(SCHED_ROUND_ROBIN);
  print_stats();
  exit(0);
}
