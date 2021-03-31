#include <libc.h>
#include "cmos.h"
#include "cpu.h"

#define CMOS_INDEX_PORT 0x70
#define CMOS_DATA_PORT 0x71

#define CMOS_NMI_BIT 0x40

#define NMI_ENABLE(data)    \
    ((data) | (CMOS_NMI_BIT))
#define NMI_DISABLE(data)   \
    ((data) & (~(CMOS_NMI_BIT)))

/* these are in BCD */
#define RTC_CUR_SEC 0x0
#define RTC_ALARM_SEC 0x1
#define RTC_CUR_MIN 0x2
#define RTC_ALARM_MIN 0x3
#define RTC_CUR_HOUR 0x4
#define RTC_ALARM_HOUR 0x5
#define RTC_DAY_OF_WEEK 0x6
#define RTC_DAY_OF_MONTH 0x7
#define RTC_MONTH 0x8
#define RTC_YEAR 0x9

#define STATUS_REG_A 0xA
#define STATUS_REG_A_UPDATE_IN_PROGRESS 0x80
#define STATUS_REG_B 0xB
#define STATUS_REG_C 0xC
#define STATUS_REG_D 0xD

#define CMOS_DIAGNOSTIC_STATUS 0xE
#define CMOS_DIAGNOSTIC_RTC_LOST_POWER 0x80

#define CMOS_SHUTDOWN_STATUS 0xF

#define BCD_TO_INT(bcd) \
    (((((bcd) & 0xF0) >> 4) * 10) + ((bcd) & 0xF))

/*
 * CMOS OUT port
 */
static int cmos_select_ram(int idx)
{
    outportb(CMOS_INDEX_PORT, idx);
    return 0;
}

/*
 * CMOS IN port
 */
inline static char cmos_read_ram()
{
    return inportb(CMOS_DATA_PORT);
}

inline static int cmos_write_ram(int data)
{
    outportb(CMOS_DATA_PORT, data);
    return 0;
}