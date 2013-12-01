/*
 * launch.h
 *
 * Utility function to launch a binary and collect
 * the return code, stdin and stdout
 *
 * See LICENSE.md for Copyright information
 */
#ifndef POLYSQUARE_CPP_SUBPROCESS_LAUNCH_H
#define POLYSQUARE_CPP_SUBPROCESS_LAUNCH_H

#include <string>
#include <unistd.h>

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem;

        pid_t launchBinary (std::string     const &executable,
                            char            const **argv,
                            char            const **environment,
                            int                   &stderrWriteEnd,
                            int                   &stdoutWriteEnd,
                            OperatingSystem const &os);
        int waitForProcessEnd (pid_t                 process,
                               OperatingSystem const &os);
        int launchBinaryAndWaitForReturn (const std::string     &executable,
                                          const char            **argv,
                                          const char            **environment,
                                          int                   &stderrWriteEnd,
                                          int                   &stdoutWriteEnd,
                                          OperatingSystem const &os);
    }
}


#endif
