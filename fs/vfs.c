// Virtual filesystem

//includes
#include <linklist.h>
#include <error.h>
#include <floppy.h>
#include "vfs.h"

struct mount_point
{
    struct llist_t ll;
    //filename
    char *name
    //a struct with storage device driver provided routins to read and manipulate data on it.
    struct fs_driver *fs_driver;
};

struct vfs
{
    struct mount_point **mount_pts;
    size_t dir_count;
};

static struct vfs _vfs;

int vfs_init()
{
    _vfs.mount_pts = NULL;
    _vfs.dir_count = 0;

    return 0;
}
