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
public:
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
    public:
        ~Buffer();

        int mode() const { return m_mode; }
        void setbuf(u8* data, int mode, size_t size);

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

    private:
        u8* m_data { nullptr };
        size_t m_capacity { BUFSIZ };
        size_t m_begin { 0 };
        size_t m_end { 0 };

        int m_mode { -1 };
        u8 m_unget_buffer { 0 };
        bool m_ungotten : 1 { false };
        bool m_data_is_malloced : 1 { false };
        bool m_empty : 1 { true };
    };

    ssize_t do_read(u8*, size_t);
    ssize_t do_write(const u8*, size_t);

    bool read_into_buffer();
    bool write_from_buffer();

    void lock();
    void unlock();

    int m_fd { -1 };
    int m_mode { 0 };
    u8 m_flags { Flags::None };
    int m_error { 0 };
    bool m_eof { false };
    pid_t m_popen_child { -1 };
    Buffer m_buffer;
    __pthread_mutex_t m_mutex;

    friend class ScopedFileLock;
};

FILE::~FILE()
{
    bool already_closed = m_fd == -1;
    VERIFY(already_closed);
}

FILE* FILE::create(int fd, int mode)
{
    void* file = calloc(1, sizeof(FILE));
    new (file) FILE(fd, mode);
    return (FILE*)file;
}

bool FILE::close()
{
    bool flush_ok = flush();
    int rc = ::close(m_fd);
    m_fd = -1;
    if (!flush_ok) {
        errno = m_error;
    }
    return flush_ok && rc == 0;
}

bool FILE::flush()
{
    if (m_mode & O_WRONLY && m_buffer.may_use()) {

        while (m_buffer.is_not_empty()) {
            bool ok = write_from_buffer();
            if (!ok)
                return false;
        }
    }
    if (m_mode & O_RDONLY) {
        VERIFY(m_buffer.buffered_size() <= NumericLimits<off_t>::max());
        off_t had_buffered = m_buffer.buffered_size();
        m_buffer.drop();
        if (lseek(m_fd, -had_buffered, SEEK_CUR) < 0) {
            if (errno == ESPIPE) {
                errno = 0;
            } else {
                return false;
            }
        }
    }

    return true;
}

void FILE::purge()
{
    m_buffer.drop();
}

ssize_t FILE::do_read(u8* data, size_t size)
{
    int nread = ::read(m_fd, data, size);

    if (nread < 0) {
        m_error = errno;
    } else if (nread == 0) {
        m_eof = true;
    }
    return nread;
}

ssize_t FILE::do_write(const u8* data, size_t size)
{
    int nwritten = ::write(m_fd, data, size);

    if (nwritten < 0)
        m_error = errno;
    
    return nwritten;
}

bool FILE::read_into_buffer()
{
    m_buffer.realize(m_fd);

    size_t available_size;
    u8* data = m_buffer.begin_enqueue(available_size);
    VERIFY(available_size);

    ssize_t nread = do_read(data, available_size);

    if (nread <= 0)
        return false;

    m_buffer.did_enqueue(nread);
    return true;
}

bool FILE::write_from_buffer()
{
    size_t size;
    const u8* data = m_buffer.begin_dequeue(size);
    // If we want to write, the buffer must have something in it!
    VERIFY(size);

    ssize_t nwritten = do_write(data, size);

    if (nwritten < 0)
        return false;

    m_buffer.did_dequeue(nwritten);
    return true;
}

size_t FILE::read(u8* data, size_t size)
{
    size_t total_read = 0;

    m_flags |= Flags::LastRead;
    m_flags &= ~Flags::LastWrite;

    while (size > 0) {
        size_t actual_size;

        if (m_buffer.may_use()) {
            // Let's see if the buffer has something queued for us.
            size_t queued_size;
            const u8* queued_data = m_buffer.begin_dequeue(queued_size);
            if (queued_size == 0) {
                // Nothing buffered; we're going to have to read some.
                bool read_some_more = read_into_buffer();
                if (read_some_more) {
                    // Great, now try this again.
                    continue;
                }
                return total_read;
            }
            actual_size = min(size, queued_size);
            memcpy(data, queued_data, actual_size);
            m_buffer.did_dequeue(actual_size);
        } else {
            // Read directly into the user buffer.
            ssize_t nread = do_read(data, size);
            if (nread <= 0)
                return total_read;
            actual_size = nread;
        }

        total_read += actual_size;
        data += actual_size;
        size -= actual_size;
    }

    return total_read;
}

size_t FILE::write(const u8* data, size_t size)
{
    size_t total_written = 0;

    m_flags &= ~Flags::LastRead;
    m_flags |= Flags::LastWrite;

    while (size > 0) {
        size_t actual_size;

        if (m_buffer.may_use()) {
            m_buffer.realize(m_fd);
            // Try writing into the buffer.
            size_t available_size;
            u8* buffer_data = m_buffer.begin_enqueue(available_size);
            if (available_size == 0) {
                // There's no space in the buffer; we're going to free some.
                bool freed_some_space = write_from_buffer();
                if (freed_some_space) {
                    // Great, now try this again.
                    continue;
                }
                return total_written;
            }
            actual_size = min(size, available_size);
            memcpy(buffer_data, data, actual_size);
            m_buffer.did_enqueue(actual_size);
            // See if we have to flush it.
            if (m_buffer.mode() == _IOLBF) {
                bool includes_newline = memchr(data, '\n', actual_size);
                if (includes_newline)
                    flush();
            }
        } else {
            // Write directly from the user buffer.
            ssize_t nwritten = do_write(data, size);
            if (nwritten < 0)
                return total_written;
            actual_size = nwritten;
        }

        total_written += actual_size;
        data += actual_size;
        size -= actual_size;
    }

    return total_written;
}

bool FILE::gets(u8* data, size_t size)
{
    // gets() is a lot like read(), but it is different enough in how it
    // processes newlines and null-terminates the buffer that it deserves a
    // separate implementation.
    size_t total_read = 0;

    if (size == 0)
        return false;

    m_flags |= Flags::LastRead;
    m_flags &= ~Flags::LastWrite;

    while (size > 1) {
        if (m_buffer.may_use()) {
            // Let's see if the buffer has something queued for us.
            size_t queued_size;
            const u8* queued_data = m_buffer.begin_dequeue(queued_size);
            if (queued_size == 0) {
                // Nothing buffered; we're going to have to read some.
                bool read_some_more = read_into_buffer();
                if (read_some_more) {
                    // Great, now try this again.
                    continue;
                }
                *data = 0;
                return total_read > 0;
            }
            size_t actual_size = min(size - 1, queued_size);
            u8* newline = reinterpret_cast<u8*>(memchr(queued_data, '\n', actual_size));
            if (newline)
                actual_size = newline - queued_data + 1;
            memcpy(data, queued_data, actual_size);
            m_buffer.did_dequeue(actual_size);
            total_read += actual_size;
            data += actual_size;
            size -= actual_size;
            if (newline)
                break;
        } else {
            // Sadly, we have to actually read these characters one by one.
            u8 byte;
            ssize_t nread = do_read(&byte, 1);
            if (nread <= 0) {
                *data = 0;
                return total_read > 0;
            }
            VERIFY(nread == 1);
            *data = byte;
            total_read++;
            data++;
            size--;
            if (byte == '\n')
                break;
        }
    }

    *data = 0;
    return total_read > 0;
}

int FILE::seek(off_t offset, int whence)
{
    bool ok = flush();
    if (!ok)
        return -1;

    off_t off = lseek(m_fd, offset, whence);
    if (off < 0) {
        // Note: do not set m_error.
        return off;
    }

    m_eof = false;
    return 0;
}

off_t FILE::tell()
{
    bool ok = flush();
    if (!ok)
        return -1;

    return lseek(m_fd, 0, SEEK_CUR);
}

void FILE::reopen(int fd, int mode)
{

    flush();
    close();

    m_buffer.drop();

    m_fd = fd;
    m_mode = mode;
    m_error = 0;
    m_eof = false;
}

FILE::Buffer::~Buffer()
{
    if (m_data_is_malloced)
        free(m_data);
}

void FILE::Buffer::realize(int fd)
{
    if (m_mode == -1)
        m_mode = isatty(fd) ? _IOLBF : _IOFBF;

    if (m_mode != _IONBF && m_data == nullptr) {
        m_data = reinterpret_cast<u8*>(malloc(m_capacity));
        m_data_is_malloced = true;
    }
}

void FILE::Buffer::setbuf(u8* data, int mode, size_t size)
{
    drop();
    m_mode = mode;
    if (data != nullptr) {
        m_data = data;
        m_capacity = size;
    }
}

void FILE::Buffer::drop()
{
    if (m_data_is_malloced) {
        free(m_data);
        m_data = nullptr;
        m_data_is_malloced = false;
    }
    m_begin = m_end = 0;
    m_empty = true;
    m_ungotten = false;
}

size_t FILE::Buffer::buffered_size() const
{
    // Note: does not include the ungetc() buffer.

    if (m_empty)
        return 0;

    if (m_begin < m_end)
        return m_end - m_begin;
    else
        return m_capacity - (m_begin - m_end);
}

const u8* FILE::Buffer::begin_dequeue(size_t& available_size) const
{
    if (m_ungotten) {
        available_size = 1;
        return &m_unget_buffer;
    }

    if (m_empty) {
        available_size = 0;
        return nullptr;
    }

    if (m_begin < m_end)
        available_size = m_end - m_begin;
    else
        available_size = m_capacity - m_begin;

    return &m_data[m_begin];
}

void FILE::Buffer::did_dequeue(size_t actual_size)
{
    VERIFY(actual_size > 0);

    if (m_ungotten) {
        VERIFY(actual_size == 1);
        m_ungotten = false;
        return;
    }

    m_begin += actual_size;

    VERIFY(m_begin <= m_capacity);
    if (m_begin == m_capacity) {
        // Wrap around.
        m_begin = 0;
    }

    if (m_begin == m_end) {
        m_empty = true;
        m_begin = m_end = 0;
    }
}

u8* FILE::Buffer::begin_enqueue(size_t& available_size) const
{
    VERIFY(m_data != nullptr);

    if (m_begin < m_end || m_empty)
        available_size = m_capacity - m_end;
    else
        available_size = m_begin - m_end;

    return const_cast<u8*>(&m_data[m_end]);
}

void FILE::Buffer::did_enqueue(size_t actual_size)
{
    VERIFY(m_data != nullptr);
    VERIFY(actual_size > 0);

    m_end += actual_size;

    VERIFY(m_end <= m_capacity);
    if (m_end == m_capacity) {

        m_end = 0;
    }

    m_empty = false;
}

bool FILE::Buffer::enqueue_front(u8 byte)
{
    if (m_ungotten) {
        return false;
    }

    m_ungotten = true;
    m_unget_buffer = byte;
    return true;
}

void FILE::lock()
{
    __pthread_mutex_lock(&m_mutex);
}

void FILE::unlock()
{
    __pthread_mutex_unlock(&m_mutex);
}

class ScopedFileLock {
public:
    ScopedFileLock(FILE* file)
        : m_file(file)
    {
        m_file->lock();
    }

    ~ScopedFileLock()
    {
        m_file->unlock();
    }

private:
    FILE* m_file;
};

extern "C" {

static u8 default_streams[3][sizeof(FILE)];
FILE* stdin = reinterpret_cast<FILE*>(&default_streams[0]);
FILE* stdout = reinterpret_cast<FILE*>(&default_streams[1]);
FILE* stderr = reinterpret_cast<FILE*>(&default_streams[2]);

void __stdio_init()
{
    new (stdin) FILE(0, O_RDONLY);
    new (stdout) FILE(1, O_WRONLY);
    new (stderr) FILE(2, O_WRONLY);
    stderr->setbuf(nullptr, _IONBF, 0);
    __stdio_is_initialized = true;
}

int setvbuf(FILE* stream, char* buf, int mode, size_t size)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF) {
        errno = EINVAL;
        return -1;
    }
    stream->setbuf(reinterpret_cast<u8*>(buf), mode, size);
    return 0;
}

void setbuf(FILE* stream, char* buf)
{
    setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}

void setlinebuf(FILE* stream)
{
    setvbuf(stream, nullptr, _IOLBF, 0);
}

int fileno(FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->fileno();
}

int feof(FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->eof();
}

int fflush(FILE* stream)
{
    if (!stream) {
        dbgln("FIXME: fflush(nullptr) should flush all open streams");
        return 0;
    }
    ScopedFileLock lock(stream);
    return stream->flush() ? 0 : EOF;
}

char* fgets(char* buffer, int size, FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    bool ok = stream->gets(reinterpret_cast<u8*>(buffer), size);
    return ok ? buffer : nullptr;
}

int fgetc(FILE* stream)
{
    VERIFY(stream);
    char ch;
    size_t nread = fread(&ch, sizeof(char), 1, stream);
    if (nread == 1)
        return ch;
    return EOF;
}

int fgetc_unlocked(FILE* stream)
{
    VERIFY(stream);
    char ch;
    size_t nread = fread_unlocked(&ch, sizeof(char), 1, stream);
    if (nread == 1)
        return ch;
    return EOF;
}

int getc(FILE* stream)
{
    return fgetc(stream);
}

int getc_unlocked(FILE* stream)
{
    return fgetc_unlocked(stream);
}

int getchar()
{
    return getc(stdin);
}

ssize_t getdelim(char** lineptr, size_t* n, int delim, FILE* stream)
{
    if (!lineptr || !n) {
        errno = EINVAL;
        return -1;
    }

    if (*lineptr == nullptr || *n == 0) {
        *n = BUFSIZ;
        if ((*lineptr = static_cast<char*>(malloc(*n))) == nullptr) {
            return -1;
        }
    }

    char* ptr;
    char* eptr;
    for (ptr = *lineptr, eptr = *lineptr + *n;;) {
        int c = fgetc(stream);
        if (c == -1) {
            if (feof(stream)) {
                *ptr = '\0';
                return ptr == *lineptr ? -1 : ptr - *lineptr;
            } else {
                return -1;
            }
        }
        *ptr++ = c;
        if (c == delim) {
            *ptr = '\0';
            return ptr - *lineptr;
        }
        if (ptr + 2 >= eptr) {
            char* nbuf;
            size_t nbuf_sz = *n * 2;
            ssize_t d = ptr - *lineptr;
            if ((nbuf = static_cast<char*>(realloc(*lineptr, nbuf_sz))) == nullptr) {
                return -1;
            }
            *lineptr = nbuf;
            *n = nbuf_sz;
            eptr = nbuf + nbuf_sz;
            ptr = nbuf + d;
        }
    }
}

ssize_t getline(char** lineptr, size_t* n, FILE* stream)
{
    return getdelim(lineptr, n, '\n', stream);
}

int ungetc(int c, FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    bool ok = stream->ungetc(c);
    return ok ? c : EOF;
}

int fputc(int ch, FILE* stream)
{
    VERIFY(stream);
    u8 byte = ch;
    ScopedFileLock lock(stream);
    size_t nwritten = stream->write(&byte, 1);
    if (nwritten == 0)
        return EOF;
    VERIFY(nwritten == 1);
    return byte;
}

int putc(int ch, FILE* stream)
{
    return fputc(ch, stream);
}

int putchar(int ch)
{
    return putc(ch, stdout);
}

int fputs(const char* s, FILE* stream)
{
    VERIFY(stream);
    size_t len = strlen(s);
    ScopedFileLock lock(stream);
    size_t nwritten = stream->write(reinterpret_cast<const u8*>(s), len);
    if (nwritten < len)
        return EOF;
    return 1;
}

int puts(const char* s)
{
    int rc = fputs(s, stdout);
    if (rc == EOF)
        return EOF;
    return fputc('\n', stdout);
}

void clearerr(FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    stream->clear_err();
}

int ferror(FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->error();
}

size_t fread_unlocked(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    VERIFY(stream);
    VERIFY(!Checked<size_t>::multiplication_would_overflow(size, nmemb));

    size_t nread = stream->read(reinterpret_cast<u8*>(ptr), size * nmemb);
    if (!nread)
        return 0;
    return nread / size;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return fread_unlocked(ptr, size, nmemb, stream);
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    VERIFY(stream);
    VERIFY(!Checked<size_t>::multiplication_would_overflow(size, nmemb));

    ScopedFileLock lock(stream);
    size_t nwritten = stream->write(reinterpret_cast<const u8*>(ptr), size * nmemb);
    if (!nwritten)
        return 0;
    return nwritten / size;
}

int fseek(FILE* stream, long offset, int whence)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->seek(offset, whence);
}

int fseeko(FILE* stream, off_t offset, int whence)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->seek(offset, whence);
}

long ftell(FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->tell();
}

off_t ftello(FILE* stream)
{
    VERIFY(stream);
    ScopedFileLock lock(stream);
    return stream->tell();
}

int fgetpos(FILE* stream, fpos_t* pos)
{
    VERIFY(stream);
    VERIFY(pos);

    ScopedFileLock lock(stream);
    off_t val = stream->tell();
    if (val == -1L)
        return 1;

    *pos = val;
    return 0;
}

int fsetpos(FILE* stream, const fpos_t* pos)
{
    VERIFY(stream);
    VERIFY(pos);

    ScopedFileLock lock(stream);
    return stream->seek(*pos, SEEK_SET);
}

void rewind(FILE* stream)
{
    fseek(stream, 0, SEEK_SET);
    clearerr(stream);
}

ALWAYS_INLINE void stdout_putch(char*&, char ch)
{
    putchar(ch);
}

static FILE* __current_stream = nullptr;
ALWAYS_INLINE static void stream_putch(char*&, char ch)
{
    fputc(ch, __current_stream);
}

int vfprintf(FILE* stream, const char* fmt, va_list ap)
{
    __current_stream = stream;
    return printf_internal(stream_putch, nullptr, fmt, ap);
}

int fprintf(FILE* stream, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vfprintf(stream, fmt, ap);
    va_end(ap);
    return ret;
}

int vprintf(const char* fmt, va_list ap)
{
    return printf_internal(stdout_putch, nullptr, fmt, ap);
}

int printf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
}

int vasprintf(char** strp, const char* fmt, va_list ap)
{
    StringBuilder builder;
    builder.appendvf(fmt, ap);
    VERIFY(builder.length() <= NumericLimits<int>::max());
    int lenght = builder.length();
    *strp = strdup(builder.to_string().characters());
    return lenght;
}

int asprintf(char** strp, const char* fmt, ...)
{
    StringBuilder builder;
    va_list ap;
    va_start(ap, fmt);
    builder.appendvf(fmt, ap);
    va_end(ap);
    VERIFY(builder.length() <= NumericLimits<int>::max());
    int length = builder.length();
    *strp = strdup(builder.to_string().characters());
    return length;
}

static void buffer_putch(char*& bufptr, char ch)
{
    *bufptr++ = ch;
}

int vsprintf(char* buffer, const char* fmt, va_list ap)
{
    int ret = printf_internal(buffer_putch, buffer, fmt, ap);
    buffer[ret] = '\0';
    return ret;
}

int sprintf(char* buffer, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(buffer, fmt, ap)
    va_end(ap);
    return ret;
}

static size_t __vsnprintf_space_remaining;
ALWAYS_INLINE void sized_buffer_putch(char*& bufptr, char ch)
{
    if (__vsnprintf_space_remaining) {
        *bufptr++ = ch;
        --__vsnprintf_space_remaining;
    }
}