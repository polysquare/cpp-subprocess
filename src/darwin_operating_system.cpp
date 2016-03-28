/* /src/darwin_operating_system.cpp
 *
 * Implementation of UNIX system calls for Darwin
 *
 * See /LICENCE.md for Copyright information */

#include <unistd.h>
#include <stddef.h>

#include <sys/fcntl.h>
#include <sys/wait.h>

#include <poll.h>

#include <cpp-subprocess/operating_system.h>

namespace ps = polysquare::subprocess;

namespace
{
    class DarwinOperatingSystem :
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
DarwinOperatingSystem::pipe (int p[2]) const
{
    int ret = ::pipe (p);

    /* Darwin doesn't provide a race-free mechanism
     * to set O_CLOEXEC, so we need to emulate it here */
    if (ret == 0)
    {
        ::fcntl (p[0], F_SETFD, O_CLOEXEC);
        ::fcntl (p[1], F_SETFD, O_CLOEXEC);
    }

    return ret;
}

int
DarwinOperatingSystem::execve (char const   *path,
                               char * const argv[],
                               char * const envp[]) const
{
    return ::execve (path, argv, envp);
}

int
DarwinOperatingSystem::access (char const *path, int mode) const
{
    return ::access (path, mode);
}

int
DarwinOperatingSystem::dup (int fd) const
{
    return ::dup (fd);
}

int
DarwinOperatingSystem::dup2 (int fd, int fd2) const
{
    return ::dup2 (fd, fd2);
}

int
DarwinOperatingSystem::close (int fd) const
{
    return ::close (fd);
}

pid_t
DarwinOperatingSystem::waitpid (pid_t pid,
                                int   *status,
                                int   options) const
{
    return ::waitpid (pid, status, options);
}

pid_t
DarwinOperatingSystem::fork () const
{
    return ::fork ();
}

int
DarwinOperatingSystem::poll (struct pollfd *pfd, int n, int timeout) const
{
    return ::poll (pfd, n, timeout);
}

ssize_t
DarwinOperatingSystem::read (int fd, void *msg, size_t n) const
{
    return ::read (fd, msg, n);
}

ssize_t
DarwinOperatingSystem::write (int fd, void *msg, size_t n) const
{
    return ::write (fd, msg, n);
}

int
DarwinOperatingSystem::fcntl_setfl (int fd, int flags) const
{
    return ::fcntl (fd, F_SETFL, flags);
}

int
DarwinOperatingSystem::fcntl_getfl (int fd) const
{
    return ::fcntl (fd, F_GETFL);
}

ps::OperatingSystem::Unique
ps::MakeOperatingSystem ()
{
    return ps::OperatingSystem::Unique (new DarwinOperatingSystem ());
}
