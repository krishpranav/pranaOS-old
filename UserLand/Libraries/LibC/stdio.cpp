/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Format.h>
#include <AK/PrintfImplementation.h>
#include <AK/ScopedValueRollback.h>
#include <AK/StdLibExtras.h>
#include <AK/String.h>
#include <LibC/bits/pthread_integration.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>

struct FILE {
    FILE(int fd, int mode)
        : m_fd(fd)
        , m_mode(mode)
    {
        __pthread_mutex_init(&m_mutex, nullptr);
    }
    ~FILE();

    static FILE* create(int fd, int mode);

    void setbuf(u8* data, int mode, size_t size) { m_buffer.setbuf(data, mode, size); }

    bool flush();
    void purge();
    bool close();

    int fileno() const { return m_fd; }
    bool eof() const { return m_eof; }
    int mode() const { return m_mode; }
    u8 flags() const { return m_flags; }

    int error() const { return m_error; }
    void clear_err() { m_error = 0; }

    size_t read(u8*, size_t);
    size_t write(const u8*, size_t);

    bool gets(u8*, size_t);
    bool ungetc(u8 byte) { return m_buffer.enqueue_front(byte); }

    int seek(off_t offset, int whence);
    off_t tell();

    pid_t popen_child() { return m_popen_child; }
    void set_popen_child(pid_t child_pid) { m_popen_child = child_pid; }

    void reopen(int fd, int mode);

    enum Flags : u8 {
        None = 0,
        LastRead = 1,
        LastWrite = 2,
    };

private:
    struct Buffer {
        // A ringbuffer that also transparently implements ungetc().
    public:
        ~Buffer();

        int mode() const { return m_mode; }
        void setbuf(u8* data, int mode, size_t size);
        // Make sure to call realize() before enqueuing any data.
        // Dequeuing can be attempted without it.
        void realize(int fd);
        void drop();

        bool may_use() const { return m_ungotten || m_mode != _IONBF; }
        bool is_not_empty() const { return m_ungotten || !m_empty; }
        size_t buffered_size() const;

        const u8* begin_dequeue(size_t& available_size) const;
        void did_dequeue(size_t actual_size);

        u8* begin_enqueue(size_t& available_size) const;
        void did_enqueue(size_t actual_size);

        bool enqueue_front(u8 byte);

}