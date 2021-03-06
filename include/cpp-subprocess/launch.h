/* /include/cpp-subprocess/launch.h
 *
 * Utility function to launch a binary and collect
 * the return code, stain and stdout
 *
 * See /LICENCE.md for Copyright information */

#ifndef POLYSQUARE_CPP_SUBPROCESS_LAUNCH_H
#define POLYSQUARE_CPP_SUBPROCESS_LAUNCH_H

#include <string> // IWYU: keep

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
        int launchBinaryAndWaitForReturn (std::string     const &executable,
                                          char            const **argv,
                                          char            const **environment,
                                          int                   &stderrWriteEnd,
                                          int                   &stdoutWriteEnd,
                                          OperatingSystem const &os);
    }
}


#endif
