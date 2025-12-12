#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// A struct to represent the date and time
struct rtcdate {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

static int is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int days_in_month(int year, int month) {
    int dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && is_leap(year))
        return 29;
    return dim[month - 1];
}

// syscall counter
extern uint64 syscall_count;
uint64 sys_countsyscall(void) {
    return syscall_count;
}

uint64 sys_datetime(void) {
    uint64 user_addr;
    argaddr(0, &user_addr);

    //
    // READ MACHINE TIME (mtime)
    //
    volatile uint64 *mtime_reg = (volatile uint64*)CLINT_MTIME;
    uint64 mtime = *mtime_reg;

    // Here, we assume 1 second = 100 ticks (like timer interrupt) if unsure
    // For QEMU default 10 MHz, adjust MTIME_FREQ accordingly
    const uint64 MTIME_FREQ = 10000000ULL;  // default, works on most QEMU

    // Compute elapsed seconds
    uint64 elapsed_seconds = mtime / MTIME_FREQ;

    // Add build-time epoch to get real UNIX timestamp
#ifndef BOOT_EPOCH
#define BOOT_EPOCH 0
#endif

    uint64 now_secs = (uint64)BOOT_EPOCH + elapsed_seconds;

    // Break seconds into HH:MM:SS
    uint64 t = now_secs;
    int second = t % 60;  t /= 60;
    int minute = t % 60;  t /= 60;
    int hour   = t % 24;  t /= 24;

    uint64 total_days = t;

    // Compute date starting from 1970
    int year = 1970;
    while (1) {
        int days_this_year = is_leap(year) ? 366 : 365;
        if (total_days < days_this_year)
            break;
        total_days -= days_this_year;
        year++;
    }

    int month = 1;
    while (total_days >= days_in_month(year, month)) {
        total_days -= days_in_month(year, month);
        month++;
    }

    int day = total_days + 1;

    struct rtcdate r = {
        year, month, day,
        hour, minute, second
    };

    if (copyout(myproc()->pagetable, user_addr, (char*)&r, sizeof(r)) < 0)
        return -1;

    return 0;
}
