/*
 * mock_operating_system.h
 *
 * Mocked out system calls.
 *
 * See LICENCE.md for Copyright information.
 */

#ifndef POLYSQUARE_SUBPROCESS_MOCK_OPERATING_SYSTEM_H
#define POLYSQUARE_SUBPROCESS_MOCK_OPERATING_SYSTEM_H

#include <gmock/gmock.h>

#include <cpp-subprocess/operating_system.h>

namespace polysquare
{
    namespace subprocess
    {
        namespace mocks
        {
            class OperatingSystem :
                public subprocess::OperatingSystem
            {
                public:

                    OperatingSystem ();
                    ~OperatingSystem ();

                    MOCK_CONST_METHOD1 (pipe, int (int p[2]));
                    MOCK_CONST_METHOD3 (execve, int (char const *path,
                                                  char * const argv[],
                                                  char * const envp[]));
                    MOCK_CONST_METHOD2 (access, int (char const *path,
                                                  int         mode));
                    MOCK_CONST_METHOD1 (dup, int (int fd));
                    MOCK_CONST_METHOD2 (dup2, int (int fd, int fd2));
                    MOCK_CONST_METHOD1 (close, int (int fd));
                    MOCK_CONST_METHOD3 (waitpid, pid_t (pid_t pid,
                                                     int   *status_loc,
                                                     int   options));
                    MOCK_CONST_METHOD0 (fork, pid_t ());
                    MOCK_CONST_METHOD3 (poll, int (struct pollfd *pfd,
                                                int           nfd,
                                                int           timeout));
                    MOCK_CONST_METHOD3 (read, ssize_t (int    fd,
                                                    void   *msg,
                                                    size_t n));
                    MOCK_CONST_METHOD3 (write, ssize_t (int    fd,
                                                     void   *msg,
                                                     size_t n));
                    MOCK_CONST_METHOD2 (fcntl_setfl, int (int fd, int flags));
                    MOCK_CONST_METHOD1 (fcntl_getfl, int (int fd));

                    void IgnoreAllCalls ();
            };
        }
    }
}

#endif
