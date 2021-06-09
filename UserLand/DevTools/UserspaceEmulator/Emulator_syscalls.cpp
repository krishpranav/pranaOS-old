/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "Emulator.h"
#include "MmapRegion.h"
#include "SimpleRegion.h"
#include <AK/Debug.h>
#include <AK/Format.h>
#include <fcntl.h>
#include <sched.h>
#include <serenity.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <syscall.h>
#include <termios.h>

#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC optimize("O3")
#endif

namespace UserspaceEmulator {

u32 Emulator::virt_syscall(u32 function, u32 arg1, u32 arg2, u32 arg3)
{
    if constexpr (SPAM_DEBUG)
        reportln("Syscall: {} ({:x})", Syscall::to_string((Syscall::Function)function), function);
    switch (function) {
    case SC_chdir:
        return virt$chdir(arg1, arg2);
    case SC_dup2:
        return virt$dup2(arg1, arg2);
    case SC_get_stack_bounds:
        return virt$get_stack_bounds(arg1, arg2);
    case SC_access:
        return virt$access(arg1, arg2, arg3);
    case SC_waitid:
        return virt$waitid(arg1);
    case SC_getcwd:
        return virt$getcwd(arg1, arg2);
    case SC_ttyname:
        return virt$ttyname(arg1, arg2, arg3);
    case SC_getpgrp:
        return virt$getpgrp();
    case SC_getpgid:
        return virt$getpgid(arg1);
    case SC_setpgid:
        return virt$setpgid(arg1, arg2);
    case SC_execve:
        return virt$execve(arg1);
    case SC_sigaction:
        return virt$sigaction(arg1, arg2, arg3);
    case SC_sigreturn:
        return virt$sigreturn();
    case SC_stat:
        return virt$stat(arg1);
    case SC_realpath:
        return virt$realpath(arg1);
    case SC_gethostname:
        return virt$gethostname(arg1, arg2);
    case SC_ioctl:
        return virt$ioctl(arg1, arg2, arg3);
    case SC_get_dir_entries:
        return virt$get_dir_entries(arg1, arg2, arg3);
    case SC_profiling_enable:
        return virt$profiling_enable(arg1);
    case SC_profiling_disable:
        return virt$profiling_disable(arg1);
    case SC_disown:
        return virt$disown(arg1);
    case SC_purge:
        return virt$purge(arg1);
    case SC_mmap:
        return virt$mmap(arg1);
    case SC_mount:
        return virt$mount(arg1);
    case SC_munmap:
        return virt$munmap(arg1, arg2);
    case SC_mremap:
        return virt$mremap(arg1);
    case SC_gettid:
        return virt$gettid();
    case SC_getpid:
        return virt$getpid();
    case SC_getsid:
        return virt$getsid(arg1);
    case SC_pledge:
        return virt$pledge(arg1);
    case SC_unveil:
        return virt$unveil(arg1);
    case SC_getuid:
        return virt$getuid();
    case SC_geteuid:
        return virt$geteuid();
    case SC_getgid:
        return virt$getgid();
    case SC_getegid:
        return virt$getegid();
    case SC_setuid:
        return virt$setuid(arg1);
    case SC_setgid:
        return virt$setgid(arg2);
    case SC_close:
        return virt$close(arg1);
    case SC_fstat:
        return virt$fstat(arg1, arg2);
    case SC_mkdir:
        return virt$mkdir(arg1, arg2, arg3);
    case SC_rmdir:
        return virt$rmdir(arg1, arg2);
    case SC_unlink:
        return virt$unlink(arg1, arg2);
    case SC_symlink:
        return virt$symlink(arg1);
    case SC_rename:
        return virt$rename(arg1);
    case SC_set_coredump_metadata:
        return virt$set_coredump_metadata(arg1);
    case SC_write:
        return virt$write(arg1, arg2, arg3);
    case SC_read:
        return virt$read(arg1, arg2, arg3);
    case SC_mprotect:
        return virt$mprotect(arg1, arg2, arg3);
    case SC_madvise:
        return virt$madvise(arg1, arg2, arg3);
    case SC_anon_create:
        return virt$anon_create(arg1, arg2);
    case SC_sendfd:
        return virt$sendfd(arg1, arg2);
    case SC_recvfd:
        return virt$recvfd(arg1, arg2);
    case SC_open:
        return virt$open(arg1);
    case SC_pipe:
        return virt$pipe(arg1, arg2);
    case SC_fcntl:
        return virt$fcntl(arg1, arg2, arg3);
    case SC_getgroups:
        return virt$getgroups(arg1, arg2);
    case SC_setgroups:
        return virt$setgroups(arg1, arg2);
    case SC_lseek:
        return virt$lseek(arg1, arg2, arg3);
    case SC_socket:
        return virt$socket(arg1, arg2, arg3);
    case SC_getsockopt:
        return virt$getsockopt(arg1);
    case SC_get_process_name:
        return virt$get_process_name(arg1, arg2);
    case SC_set_process_name:
        return virt$set_process_name(arg1, arg2);
    case SC_dbgputstr:
        return virt$dbgputstr(arg1, arg2);
    case SC_dbgputch:
        return virt$dbgputch(arg1);
    case SC_chmod:
        return virt$chmod(arg1, arg2, arg3);
    case SC_fchmod:
        return virt$fchmod(arg1, arg2);
    case SC_fchown:
        return virt$fchown(arg1, arg2, arg3);
    case SC_accept4:
        return virt$accept4(arg1);
    case SC_setsockopt:
        return virt$setsockopt(arg1);
    case SC_getsockname:
        return virt$getsockname(arg1);
    case SC_getpeername:
        return virt$getpeername(arg1);
    case SC_bind:
        return virt$bind(arg1, arg2, arg3);
    case SC_connect:
        return virt$connect(arg1, arg2, arg3);
    case SC_shutdown:
        return virt$shutdown(arg1, arg2);
    case SC_listen:
        return virt$listen(arg1, arg2);
    case SC_select:
        return virt$select(arg1);
    case SC_recvmsg:
        return virt$recvmsg(arg1, arg2, arg3);
    case SC_sendmsg:
        return virt$sendmsg(arg1, arg2, arg3);
    case SC_kill:
        return virt$kill(arg1, arg2);
    case SC_killpg:
        return virt$killpg(arg1, arg2);
    case SC_set_mmap_name:
        return virt$set_mmap_name(arg1);
    case SC_sync:
        virt$sync();
        return 0;
    case SC_exit:
        virt$exit((int)arg1);
        return 0;
    case SC_gettimeofday:
        return virt$gettimeofday(arg1);
    case SC_clock_gettime:
        return virt$clock_gettime(arg1, arg2);
    case SC_clock_settime:
        return virt$clock_settime(arg1, arg2);
    case SC_getrandom:
        return virt$getrandom(arg1, arg2, arg3);
    case SC_fork:
        return virt$fork();
    case SC_emuctl:
        return virt$emuctl(arg1, arg2, arg3);
    case SC_sched_getparam:
        return virt$sched_getparam(arg1, arg2);
    case SC_sched_setparam:
        return virt$sched_setparam(arg1, arg2);
    case SC_set_thread_name:
        return virt$set_thread_name(arg1, arg2, arg3);
    case SC_setsid:
        return virt$setsid();
    case SC_create_inode_watcher:
        return virt$create_inode_watcher(arg1);
    case SC_inode_watcher_add_watch:
        return virt$inode_watcher_add_watch(arg1);
    case SC_inode_watcher_remove_watch:
        return virt$inode_watcher_remove_watch(arg1, arg2);
    case SC_clock_nanosleep:
        return virt$clock_nanosleep(arg1);
    case SC_readlink:
        return virt$readlink(arg1);
    case SC_ptsname:
        return virt$ptsname(arg1, arg2, arg3);
    case SC_allocate_tls:
        return virt$allocate_tls(arg1, arg2);
    case SC_beep:
        return virt$beep();
    case SC_ftruncate:
        return virt$ftruncate(arg1, arg2);
    case SC_umask:
        return virt$umask(arg1);
    case SC_chown:
        return virt$chown(arg1);
    case SC_msyscall:
        return virt$msyscall(arg1);
    case SC_futex:
        return virt$futex(arg1);
    default:
        reportln("\n=={}==  \033[31;1mUnimplemented syscall: {}\033[0m, {:p}", getpid(), Syscall::to_string((Syscall::Function)function), function);
        dump_backtrace();
        TODO();
    }
}
    
    int Emulator::virt$anon_create(size_t size, int options)
{
    return syscall(SC_anon_create, size, options);
}

int Emulator::virt$sendfd(int socket, int fd)
{
    return syscall(SC_sendfd, socket, fd);
}

int Emulator::virt$recvfd(int socket, int options)
{
    return syscall(SC_recvfd, socket, options);
}

int Emulator::virt$profiling_enable(pid_t pid)
{
    return syscall(SC_profiling_enable, pid);
}

int Emulator::virt$profiling_disable(pid_t pid)
{
    return syscall(SC_profiling_disable, pid);
}

int Emulator::virt$disown(pid_t pid)
{
    return syscall(SC_disown, pid);
}

int Emulator::virt$purge(int mode)
{
    return syscall(SC_purge, mode);
}

}


