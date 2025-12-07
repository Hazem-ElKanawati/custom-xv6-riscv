#include "types.h"

extern int kbd_intr_count;

uint64
sys_kbdint(void)
{
  return kbd_intr_count;
}


extern uint64 syscall_count;   // defined in syscall.c

uint64
sys_countsyscall(void)
{
    return syscall_count;
}
