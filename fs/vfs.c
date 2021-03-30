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

int mount(enum storage_dev_type dev_type, char *filename)
{
    struct mount_point *mount;

    if (!filename)
        return -EBADARG;

    /* Create a new mount_point */
    mount = (struct mount_point *) kalloc(sizeof(struct mount_point));
    if (!mount)
        return -ENOMEM;
    mount->fs_driver = (struct fs_driver *) kalloc(sizeof(struct fs_driver));
    if (!mount->fs_driver)
        return -ENOMEM;
    mount->fs_driver->dev_driver = (struct dev_driver *) kalloc(sizeof(struct dev_driver));
    if (!mount->fs_driver->dev_driver)
        return -ENOMEM;
    mount->name = (char *) kalloc(MAX_MOUNTNAME_SIZE);
    strcpy(mount->name, filename);

    /* Initialize the storage driver functions */
    switch (dev_type) {
        case STORAGE_DEVICE_FLOPPY:
            floppy_init_driver(mount->fs_driver->dev_driver);
            fat12_init_fs(mount->fs_driver);
            break;
        case STORAGE_DEVICE_HDD:
            printf("HDD storage devices are not implemented yet");
            break;
        default:
            return -EBADARG;
    }

    /* Add the mount point the the VFS directory pool */
    if (_vfs.mount_pts)
        llist_add_before(_vfs.mount_pts, mount, ll);
    else
    {
        llist_init(mount, ll);
        _vfs.mount_pts = mount;
    }
    _vfs.dir_count++;

    return 0;
}

int unmount(char *filename)
{
    return 0;
}

FILE *open(char *filename)
{
    if (!filename)
        return NULL;

    return NULL;
}

void close(FILE *hndl)
{

}

int seek(enum seek_type type, size_t offset)
{
    return -1;
}

int read(FILE *hndl, void *buf, size_t nbytes)
{
    return -1;
}
