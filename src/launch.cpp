/* /src/launch.cpp
 *
 * Utility function to launch a binary and collect
 * the return code, stain and stdout
 *
 * See /LICENCE.md for Copyright information */

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream> // IWYU pragma: keep

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> // IWYU pragma: keep

#include <sys/wait.h> // IWYU pragma: keep
#include <unistd.h>

#include <cpp-subprocess/fdbackup.h>
#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/redirectedfd.h>
#include <cpp-subprocess/launch.h> // IWYU pragma: keep

namespace ps = polysquare::subprocess;

pid_t ps::launchBinary (std::string         const &executable,
                        char                const **argv,
                        char                const **environment,
                        int                       &stderrWriteEnd,
                        int                       &stdoutWriteEnd,
                        ps::OperatingSystem const &os)
{
    FDBackup stderr (STDERR_FILENO, os);
    FDBackup stdout (STDOUT_FILENO, os);

    /* Close the originals once they have been backed up
     * We have to do this here and not in the FDBackup
     * constructors because of an order-of-operations issue -
     * namely if we close an original file descriptor name
     * before duplicating another one, then there's a possibility
     * that the duplicated other one will get the same name as
     * the one we just closed, making us unable to restore
     * the closed one properly */
    if (os.close (STDERR_FILENO) == -1)
        throw std::runtime_error (strerror (errno));

    if (os.close (STDOUT_FILENO) == -1)
        throw std::runtime_error (strerror (errno));

    /* Replace the current process stderr and stdout with the write end
     * of the pipes. Now when someone tries to write to stderr or stdout
     * they'll write to our pipes instead */
    RedirectedFD pipedStderr (STDERR_FILENO, stderrWriteEnd, os);
    RedirectedFD pipedStdout (STDOUT_FILENO, stdoutWriteEnd, os);

    /* Fork process, child gets a copy of the pipe write ends
     * - the original pipe write ends will be closed on exec
     * but the duplicated write ends now taking the place of
     * stderr and stdout will not be */
    pid_t child = os.fork ();

    /* Child process */
    if (child == 0)
    {
        if (os.execve (executable.c_str (),
                       const_cast <char * const *> (argv),
                       const_cast <char * const *> (environment)) == -1)
        {
            std::cerr << "execvpe failed with error "
                      << errno
                      << std::endl
                      << " - binary "
                      << executable
                      << std::endl;
            abort ();
        }
    }
    /* Parent process - error */
    else if (child == -1)
        throw std::runtime_error (strerror (errno));

    /* The old file descriptors for the stderr and stdout
     * are put back in place, and pipe write ends closed
     * as the child is using them at return */

    return child;
}

int ps::waitForProcessEnd (pid_t                     child,
                           ps::OperatingSystem const &os)
{
    int status = 0;

    do
    {
        /* Wait around for the child to get a signal */
        pid_t waitChild = os.waitpid (child, &status, 0);
        if (waitChild == child)
        {
            /* If it died unexpectedly, say so */
            if (WIFSIGNALED (status))
            {
                std::stringstream ss;
                ss << "child killed by signal "
                   << WTERMSIG (status);
                throw std::runtime_error (ss.str ());
            }
        }
        else
        {
            /* waitpid () failed */
            throw std::runtime_error (strerror (errno));
        }

        /* Keep going until it exited */
    } while (!WIFEXITED (status) && !WIFSIGNALED (status));

    /* Return the exit code */
    return WEXITSTATUS (status);
}

int ps::launchBinaryAndWaitForReturn (std::string     const &executable,
                                      char            const **argv,
                                      char            const **environment,
                                      int                   &stderrWriteEnd,
                                      int                   &stdoutWriteEnd,
                                      OperatingSystem const &os)
{
    pid_t child = launchBinary (executable,
                                argv,
                                environment,
                                stderrWriteEnd,
                                stdoutWriteEnd,
                                os);

    return waitForProcessEnd (child, os);
}
