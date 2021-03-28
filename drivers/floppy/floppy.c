//includes
#include <libc.h>
#include <error.h>
#include <time.h>
#include <x86/dma.h>
#include <x86/i8259.h>
#include <x86/cmos.h>
#include <x86/cpu.h>
#include <fs/vfs.h>

/*
All floppy control registers.
some of them are not usefull here, but kept just for
the sake of completeness
*/

enum FlpReg
{
    STATUS_REG_A =      0x3F0,  /* read-only */
    STATUS_REG_B =      0x3F1,  /* read-only */
    /* Digital Output Register contains the drive select and
     * motor enable bits, a reset bit and a DMA gate bit */
    DOR_REG =           0x3F2,  /* r/w */
    /* For tape support */
    TAPE_DRIVE_REG =    0x3F3,  /* right... r/w */
    MAIN_STATUS_REG =   0x3F4,  /* read-only */
    DATARATE_SELECT_REG=0x3F4,  /* write-only */
    DATA_REG =          0x3F5,  /* r/w. (FIFO) */
    DIGITAL_INPUT_REG = 0x3F7,  /* read-only */
    CTRL_REG =          0x3F7   /* write-only */
};

enum data_cmd
{
    CMD_READ_TRACK =        0x02,  /* generates IRQ6 */
    CMD_SET_PARAM =         0x03,  /* set drive parameters */
    CMD_DRIVE_STATUS =      0x04,
    CMD_WRITE_DATA =        0x05,  /* write data to disk */
    CMD_READ_DATA =         0x06,  /* read data from disk */
    CMD_RECALIBRATE =       0x07,  /* seek to cylinder 0 */
    CMD_SENSE_INTERRUPT =   0x08,  /* ack IRQ6, get status of last cmd */
    CMD_WRITE_DELETED_DATA= 0x09,
    CMD_READ_ID =           0x0A,  /* generatess IRQ6 */
    CMD_READ_DELETED_DATA = 0x0C,
    CMD_FORMAT_TRACK =      0x0D,
    CMD_SEEK =              0x0F,  /* seek both heads to cylinder X */
    CMD_VERSION =           0x10,  /* used on init */
    CMD_SCAN_EQUAL =        0x11,
    CMD_PERPENDICULAR_MODE= 0x12,  /* used on init */
    CMD_CONFIGURE =         0x13,  /* set controller parameters */
    CMD_LOCK =              0x14,  /* protect controller parameters from reset */
    CMD_VERIFY =            0x16,
    CMD_SCAN_LOW_OR_EQUAL = 0x19,
    CMD_SCAN_HIGH_OR_EQUAL= 0x1D,

    /* When read FIFO register, this value indicates that
     * an invalid command was given on the previous write */
    CMD_ERROR = 0x80
};