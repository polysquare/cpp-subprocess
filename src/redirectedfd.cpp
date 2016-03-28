/* /src/redirectedfd.cpp
 *
 * Utility class to redirect operations on one
 * file descriptor into another until destruction.
 *
 * See /LICENCE.md for Copyright information */

#include <stdexcept>
#include <iostream>

#include <string.h>

#include <errno.h> // IWYU pragma: keep

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/redirectedfd.h>

namespace ps = polysquare::subprocess;

ps::RedirectedFD::RedirectedFD (int                   from,
                                int                   &to,
                                OperatingSystem const &os) :
    mToFd (to),
    mOS (os)
{
    mOS.close (from);
    /* Make 'to' take the old file descriptor's place */
    if (mOS.dup2 (to, from) == -1)
        throw std::runtime_error (strerror (errno));
}

ps::RedirectedFD::~RedirectedFD ()
{
    if (mToFd &&
        mOS.close (mToFd) == -1)
        std::cerr << "Failed to close redirect-to file descriptor "
                  << strerror (errno) << std::endl;

    mToFd = 0;
}
