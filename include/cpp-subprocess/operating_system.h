/*
 * operating_system.h
 * Simple thin virtual class to abstract away some UNIX userland
 * implementation details.
 *
 * See LICENCE.md for Copyright information.
 */

#ifndef POLYSQUARE_SUBPROCESS_OPERATING_SYSTEM_H
#define POLYSQUARE_SUBPROCESS_OPERATING_SYSTEM_H

#include <memory>  // IWYU pragma: keep

#include <stddef.h>
#include <sys/types.h>

typedef struct pollfd pollfd_t;

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem
        {
            public:

                typedef std::unique_ptr <OperatingSystem> Unique;

                virtual ~OperatingSystem () {};

                virtual int pipe (int pipe[2]) const = 0;
                virtual int execve (char const *path,
                                    char * const argv[],
                                    char * const envp[]) const = 0;
                virtual int access (const char *path, int mode) const = 0;
                virtual int dup (int fd) const = 0;
                virtual int dup2 (int fd, int fd2) const = 0;
                virtual int close (int fd) const = 0;
                virtual pid_t waitpid (pid_t pid,
                                       int *stat_loc,
                                       int options) const = 0;
                virtual pid_t fork () const = 0;
                virtual int poll (struct pollfd *pfd,
                                  int nfd,
                                  int timeout) const = 0;
                virtual ssize_t read (int fd, void *msg, size_t n) const = 0;
                virtual ssize_t write (int fd, void *msg, size_t n) const = 0;
                virtual int fcntl_setfl (int fd, int flags) const = 0;
                virtual int fcntl_getfl (int fd) const = 0;

        };

        /* This is implemented by the currently linked into
         * implementation of OperatingSystem, used to construct
         * an OperatingSystem that is compatible with the system
         * cpp-subprocess was compiled for */
        OperatingSystem::Unique MakeOperatingSystem ();
    }
}

#endif
