#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"    // Defines NPROC
#include "memlayout.h"
#include "spinlock.h" // <--- CRITICAL: Must be before proc.h
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}



uint64
sys_shutdown(void)
{
  // Write the QEMU finisher magic value to the mapped address.
  // We expect virtual address 0x100000 to be mapped to the finisher device.
  volatile unsigned int *finisher = (volatile unsigned int *)0x100000;
  *finisher = 0x5555;   // magic value that tells QEMU to exit
  // Shouldn't return (QEMU exits); but return 0 to satisfy signature.
  return 0;
}
static uint64 rand_seed = 88172645463325252ULL; // arbitrary non-zero seed

uint64
sys_rand(void)
{
  // LCG constants (64-bit)
  // seed = seed * a + c
  rand_seed = rand_seed * 6364136223846793005ULL + 1442695040888963407ULL;

  // return a 32-bit-ish positive integer (lower bits of seed)
  // Syscall returns 64-bit in kernel; user-level prototype is `int rand(void);`
  return (uint64)(rand_seed >> 1); // shift to avoid sign bit issues
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_getptable(void)
{
  int nproc;
  uint64 ubuf;

  // fetch integer argument
  argint(0, &nproc);

  // fetch address argument
  argaddr(1, &ubuf);

  return getptable(nproc, (char*)ubuf);
}

uint64
sys_datetime(void)
{
  uint64 user_addr;
  argaddr(0, &user_addr);

  struct datetime dt;
  datetime(&dt);

  if(copyout(myproc()->pagetable, user_addr, (char*)&dt, sizeof(dt)) < 0)
    return -1;

  return 0;
}

uint64
sys_getppid(void)
{
  struct proc *p = myproc();
  if(p->parent)
    return p->parent->pid;
  return 0;   // init process has no parent
}

uint64
sys_setsched(void)
{
  int mode;

  // 1. Fetch the argument.
  // If your argint returns void, use this line:
  argint(0, &mode);

  // OR, if your argint returns int (standard xv6), use this line instead:
  // if(argint(0, &mode) < 0) return -1;

  // 2. Validate the mode
  if(mode != SCHED_ROUND_ROBIN && mode != SCHED_FCFS && mode != SCHED_PRIORITY)
    return -1;

  // 3. Set the mode
  sched_mode = mode;
  return 0;
}

/* Set a process's priority */
uint64
sys_setpriority(void)
{
  int pid, pr;
  struct proc *p;
  extern struct proc proc[NPROC]; // Fixes 'proc undeclared'

  // FIX: argint returns void, so just call it directly.
  // Do not put it inside an 'if' statement.
  argint(0, &pid);
  argint(1, &pr);

  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock);
    if(p->pid == pid && p->state != UNUSED){
      p->priority = pr;
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }

  return -1;
}
