#include "types.h"

extern int kbd_intr_count;

uint64
sys_kbdint(void)
{
  return kbd_intr_count;
}
