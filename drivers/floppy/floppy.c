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

enum data_read_mode
{
    READ_MODE_SKIP_DELETED_DATA = 0x20,
    READ_MODE_DOUBLE_DENSITY = 0x40,
    READ_MODE_MULTITRACK = 0x80
};

/*
Commands for writting to DOR_REG.
*/

enum dor_cmd
{
    /* Device selection */
    DOR_SEL_0 =     0x00,   /* 00000000 */
    DOR_SEL_1 =     0x01,   /* 00000001 */
    DOR_SEL_2 =     0x02,   /* 00000010 */
    DOR_SEL_3 =     0x03,   /* 00000011 */
    /* Clears the core circuits of 82077AA */
    DOR_RESET =     0x04,   /* 00000100 */
    /* Set floppy to DMA mode */
    DOR_DMA_GATE =  0x08,   /* 00001000 */
    /* Another device selection data.
     * It has to match with DOR_SEL_X */
    DOR_MOTOR_0 =   0x10,   /* 00010000 */
    DOR_MOTOR_1 =   0x20,   /* 00100000 */
    DOR_MOTOR_2 =   0x40,   /* 01000000 */
    DOR_MOTOR_3 =   0x80    /* 10000000 */
};