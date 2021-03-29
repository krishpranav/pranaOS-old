//includes
#include "<linklist.h>"
#include "<error.h>"
#include "fat12.h"
#include "vfs.h"


#define ROOT_DIR_OFFSET 0x2600
#define ROOT_DIR_SIZE 14 * 512 //14 sectors
#define MAX_FILES_IN_DIR 224
#define FAT12_MAX_FILENAME_LENGHT 11
