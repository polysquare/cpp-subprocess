/* /src/gnu_operating_system.cpp
 *
 * Implementation of UNIX system calls for GNU systems
 *
 * See /LICENCE.md for Copyright information */

#include "nonportable_gnu.h"  // IWYU pragma: keep

#include <stddef.h>  // IWYU pragma: keep

#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <sys/wait.h>

#include <cpp-subprocess/operating_system.h>

// IWYU pragma: no_include <bits/fcntl-linux.h>

namespace ps = polysquare::subprocess;

namespace
{
    class GNUOperatingSystem :
        public ps::OperatingSystem
    {
        public:

            int pipe (int p[2]) const;
            int execve (char const   *path,
                        char * const argv[],
                        char * const envp[]) const;
            int access (char const *path, int mode) const;
            int dup (int fd) const;
            int dup2 (int fd, int fd2) const;
            int close (int fd) const;
            pid_t waitpid (pid_t pid, int *status, int options) const;
            pid_t fork () const;
            int poll (struct pollfd *pfd, int n, int timeout) const;
            ssize_t read (int fd, void *msg, size_t n) const;
            ssize_t write (int fd, void *msg, size_t n) const;
            int fcntl_setfl (int fd, int flags) const;
            int fcntl_getfl (int fd) const;
    };
}

int
GNUOperatingSystem::pipe (int p[2]) const
{
    return ::pipe2 (p, O_CLOEXEC);
}

int
GNUOperatingSystem::execve (char const   *path,
                            char * const argv[],
                            char * const envp[]) const
{
    return ::execve (path, argv, envp);
}

int
GNUOperatingSystem::access (char const *path, int mode) const
{
    return ::access (path, mode);
}

int
GNUOperatingSystem::dup (int fd) const
{
    return ::dup (fd);
}

int
GNUOperatingSystem::dup2 (int fd, int fd2) const
{
    return ::dup2 (fd, fd2);
}

int
GNUOperatingSystem::close (int fd) const
{
    return ::close (fd);
}

pid_t
GNUOperatingSystem::waitpid (pid_t pid,
                             int   *status,
                             int   options) const
{
    return ::waitpid (pid, status, options);
}

pid_t
GNUOperatingSystem::fork () const
{
    return ::fork ();
}

int
GNUOperatingSystem::poll (struct pollfd *pfd, int n, int timeout) const
{
    return ::poll (pfd, n, timeout);
}

ssize_t
GNUOperatingSystem::read (int fd, void *msg, size_t n) const
{
    return ::read (fd, msg, n);
}

ssize_t
GNUOperatingSystem::write (int fd, void *msg, size_t n) const
{
    return ::write (fd, msg, n);
}

int
GNUOperatingSystem::fcntl_setfl (int fd, int flags) const
{
    return ::fcntl (fd, F_SETFL, flags);
}

int
GNUOperatingSystem::fcntl_getfl (int fd) const
{
    return ::fcntl (fd, F_GETFL);
}

ps::OperatingSystem::Unique
ps::MakeOperatingSystem ()
{
    return ps::OperatingSystem::Unique (new GNUOperatingSystem ());
}
