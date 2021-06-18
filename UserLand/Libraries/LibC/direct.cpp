/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <AK/ScopeGuard.h>
#include <AK/StdLibExtras.h>
#include <AK/Vector.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {


DIR* opendir(const char* name)
{
    int fd = open(name, O_RDONLY | O_DIRECTORY);
    if (fd == -1)
        return nullptr;
    DIR* dirp = (DIR*)malloc(sizeof(DIR));
    dirp->fd = fd;
    dirp->buffer = nullptr;
    dirp->buffer_size = 0;
    dirp->nextptr = nullptr;
    return dirp;
}

int closedir(DIR* dirp)
{
    if (!dirp || dirp->fd == -1)
        return -EBADF;
    free(dirp->buffer);
    int rc = close(dirp->fd);
    if (rc == 0)
        dirp->fd = -1;
    free(dirp);
    return rc;
}

void rewinddir(DIR* dirp)
{
    free(dirp->buffer);
    dirp->buffer = nullptr;
    dirp->buffer_size = 0;
    dirp->nextptr = nullptr;
    lseek(dirp->fd, 0, SEEK_SET);
}

struct [[gnu::packed]] sys_dirent {
    ino_t ino;
    u8 file_type;
    size_t namelen;
    char name[];
    size_t total_size()
    {
        return sizeof(ino_t) + sizeof(u8) + sizeof(size_t) + sizeof(char) * namelen;
    }
};

}