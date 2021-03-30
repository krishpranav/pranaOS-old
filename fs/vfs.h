#ifndef VFS_SWF400KO
#define VFS_SWF400KO

#include <libc.h>

#define MAX_MOUNTNAME_SIZE 16
#define MAX_FILENAME_LENGTH 12

enum storage_dev_type
{
    STORAGE_DEVICE_FLOPPY,
    STORAGE_DEVICE_HDD
};

enum seek_type
{
    SEEK_BEGIN,
    SEEK_OFFSET,
    SEEK_END
};

enum file_type
{
    FOLDER_TYPE,
    FILE_TYPE,
    DEVICE_TYPE
};

typedef char *dev_read_func_t(char *buf, size_t offset, size_t b_cnt);
typedef int dev_write_func_t(/* TODO: NOT IMPLEMENTED */);

struct dev_driver
{
    dev_read_func *read;
    dev_write_func_t *write;
};

typedef char *fs_read_func_t(char *buf, size_t b_cnt);
typedef int fs_write_func_t();
typedef char **fs_ls_func_t(struct fs_driver *fs_drv, const char *dir);
