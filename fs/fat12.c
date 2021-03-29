#include <linklist.h>
#include <error.h>
#include "fat12.h"
#include "vfs.h"

#define ROOT_DIR_OFFSET 0x2600
#define ROOT_DIR_SIZE 14 * 512 /* 14 sectors */
#define MAX_FILES_IN_DIR 224
#define FAT12_MAX_FILENAME_LENGTH 11

struct fat12_bootsector
{
    /* The bootsector holds quite a lot of info, but we will have just the parts
     * which interest us when working with the driver itself */

    size_t fat_cnt;
    size_t max_root_dir_cnt;
    size_t max_sector_cnt;
    size_t sectors_in_fat;
};

/* On every mount a strcture is created */
struct fat12_mount
{
    struct llist_t ll;
    /* FS driver is just to have some compare value for get_mount_point() */
    struct fs_driver *fs_drv;
    struct fat12_bootsector bootsec;
    /* Multi-dimensional array of all FAT data. FAT count is in `bootsec` */
    char **fat_data;
    /* Root directory data */
    char rootdir_data[ROOT_DIR_SIZE];
    size_t open_file_cnt;
};

struct rootdir_item
{
    char filename[11];
    unsigned char flags;
    short reserved;
    unsigned short create_time;
    unsigned short create_date;
    unsigned short last_access_date;
    unsigned short ignore_in_fat12;
    unsigned short last_write_time;
    unsigned short last_write_date;
    unsigned short fat_idx;
    size_t filesize;
};

struct fat12_mount *fat12_mounts;

static struct fat12_mount *get_mount_point(struct fs_driver *drv)
{
    struct fat12_mount *mount;
    size_t idx;

    llist_foreach(fat12_mounts, mount, idx, ll)
    {
        if (mount->fs_drv == drv)
            return mount;
    }

    return NULL;
}

static int init_bootsec(struct dev_driver *dev, struct fat12_bootsector *bootsec)
{
    char data[512];

    dev->read(data, 0, 512);
    if (!data)
        return -1;

    bootsec->fat_cnt = data[16];
    bootsec->max_root_dir_cnt = ((data[18] << 8) & 0xFF00) | (data[17] & 0xFF);
    bootsec->max_sector_cnt = ((data[20] << 8) & 0xFF00) | (data[19] & 0xFF);
    bootsec->sectors_in_fat = ((data[23] << 8) & 0xFF00) | (data[22] & 0xFF);

    return 0;
}

static char **init_fats(struct dev_driver *dev, struct fat12_bootsector *bootsec)
{
    int i;
    char **fat_buf;

    if (!dev || !bootsec)
    {
        error = -EBADARG;
        return NULL;
    }

    if (bootsec->fat_cnt == 0)
        return NULL;

    fat_buf = (char **) kalloc(sizeof(char **) * bootsec->fat_cnt);

    for (i = 0; i < bootsec->fat_cnt; i++)
    {
        fat_buf[i] = (char *) kalloc(bootsec->sectors_in_fat * 512);
        if (!fat_buf[i])
        {
            for (i--; i >= 0; i--)
                free(fat_buf[i]);
            error = -ENOMEM;
            return NULL;
        }

        dev->read(fat_buf[i], 1 + (i * bootsec->sectors_in_fat), 512 * bootsec->sectors_in_fat);
    }

    return fat_buf;
}

static char *init_rootdir(struct dev_driver *dev, struct fat12_mount *mount)
{
    if (!dev || !mount)
    {
        error = -EBADARG;
        return NULL;
    }

    dev->read(mount->rootdir_data, ROOT_DIR_OFFSET, ROOT_DIR_SIZE);

    return mount->rootdir_data;
}

static char *fs_read(FILE *file)
{
    return NULL;
}
