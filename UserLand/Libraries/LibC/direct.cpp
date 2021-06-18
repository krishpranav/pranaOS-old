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

static void create_struct_dirent(sys_dirent* sys_ent, struct dirent* str_ent)
{
    str_ent->d_ino = sys_ent->ino;
    str_ent->d_type = sys_ent->file_type;
    str_ent->d_off = 0;
    str_ent->d_reclen = sizeof(struct dirent);

    VERIFY(sizeof(str_ent->d_name) > sys_ent->namelen);

    memcpy(str_ent->d_name, sys_ent->name, sys_ent->namelen);
    str_ent->d_name[sys_ent->namelen] = '\0';
}

static int allocate_dirp_buffer(DIR* dirp)
{
    if (dirp->buffer) {
        return 0;
    }

    struct stat st;

    int old_errno = errno;
    int rc = fstat(dirp->fd, &st);
    if (rc < 0) {
        int new_errno = errno;
        errno = old_errno;
        return new_errno;
    }
    size_t size_to_allocate = max(st.st_size, static_cast<off_t>(4096));
    dirp->buffer = (char*)malloc(size_to_allocate);
    if (!dirp->buffer)
        return ENOMEM;
    for (;;) {
        ssize_t nread = syscall(SC_get_dir_entries, dirp->fd, dirp->buffer, size_to_allocate);
        if (nread < 0) {
            if (nread == -EINVAL) {
                size_to_allocate *= 2;
                char* new_buffer = (char*)realloc(dirp->buffer, size_to_allocate);
                if (new_buffer) {
                    dirp->buffer = new_buffer;
                    continue;
                } else {
                    nread = -ENOMEM;
                }
            }

            free(dirp->buffer);
            dirp->buffer = nullptr;
            return -nread;
        }
        dirp->buffer_size = nread;
        dirp->nextptr = dirp->buffer;
        break;
    }
    return 0;
}

dirent* readdir(DIR* dirp)
{
    if (!dirp)
        return nullptr;
    if (dirp->fd == -1)
        return nullptr;

    if (int new_errno = allocate_dirp_buffer(dirp)) {
        errno = new_errno;
        return nullptr;
    }

    if (dirp->nextptr >= (dirp->buffer + dirp->buffer_size))
        return nullptr;

    auto* sys_ent = (sys_dirent*)dirp->nextptr;
    create_struct_dirent(sys_ent, &dirp->cur_ent);

    dirp->nextptr += sys_ent->total_size();
    return &dirp->cur_ent;
}

static bool compare_sys_struct_dirent(sys_dirent* sys_ent, struct dirent* str_ent)
{
    size_t namelen = min((size_t)256, sys_ent->namelen);
    return sys_ent->ino == str_ent->d_ino
        && sys_ent->file_type == str_ent->d_type
        && sys_ent->total_size() == str_ent->d_reclen
        && strncmp(sys_ent->name, str_ent->d_name, namelen) == 0;
}

}