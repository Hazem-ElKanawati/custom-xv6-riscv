#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "datetime.h"

#define TICKS_PER_SEC 10000000  // QEMU cycles per second

static int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

#ifndef BOOT_EPOCH
#define BOOT_EPOCH 0   // fallback for IntelliSense only
#endif

static void unix_to_datetime(uint64 t, struct datetime *dt)
{
    uint64 sec = t % 60; t /= 60;
    uint64 min = t % 60; t /= 60;
    uint64 hour = t % 24; t /= 24;

    uint64 days = t;

    int year = 1970;
    while (1) {
        int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        int dyear = leap ? 366 : 365;
        if (days >= dyear) {
            days -= dyear;
            year++;
        } else break;
    }

    int month = 0;
    while (1) {
        int dim = mdays[month];
        if (month == 1) {
            int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            if (leap) dim = 29;
        }
        if (days >= dim) {
            days -= dim;
            month++;
        } else break;
    }

    dt->year   = year;
    dt->month  = month + 1;
    dt->day    = days + 1;
    dt->hour   = hour;
    dt->minute = min;
    dt->second = sec;
}

int datetime(struct datetime *dt)
{
    uint64 cycles = r_time();     // read cycle counter
    uint64 elapsed = cycles / TICKS_PER_SEC;
    uint64 unix_time = BOOT_EPOCH + elapsed;  // BOOT_EPOCH is macro

    unix_to_datetime(unix_time, dt);
    return 0;
}
