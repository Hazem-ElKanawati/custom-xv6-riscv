#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
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
