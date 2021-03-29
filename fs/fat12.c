//includes
#include "<linklist.h>"
#include "<error.h>"
#include "fat12.h"
#include "vfs.h"


#define ROOT_DIR_OFFSET 0x2600
#define ROOT_DIR_SIZE 14 * 512 //14 sectors
#define MAX_FILES_IN_DIR 224
#define FAT12_MAX_FILENAME_LENGHT 11

struct fat12_bootsector
{
    size_t fat_cnt;
    size_t max_root_dir_cnt;
    size_t max_sector_cnt;
    size_t sectors_in_fat;
};

struct fat12_moun{
    struct llist_t ll;

    struct fs_driver *fs_drv;
    struct fat12_bootsector bootsec;
    char **fat_data;
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
    unsigned short last_wrtie_date;
    unsigned short fat_idx;
    size_t filesize;
};
