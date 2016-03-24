/* /src/pipe.cpp
 *
 * Utility class to create a unix pipe and
 * close it on teardown
 *
 * See /LICENCE.md for Copyright information */

#include <iostream>
#include <stdexcept>

#include <string.h>

#include <errno.h> // IWYU pragma: keep

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>

namespace ps = polysquare::subprocess;

ps::Pipe::Pipe (ps::OperatingSystem const &os) :
  mOS (os)
{
    if (mOS.pipe (mPipe) == -1)
        throw std::runtime_error (strerror (errno));
}

ps::Pipe::~Pipe ()
{
    if (mPipe[0] &&
        mOS.close (mPipe[0]) == -1)
        std::cerr << "mPipe[0] close: " << strerror (errno) << std::endl;

    if (mPipe[1] &&
        mOS.close (mPipe[1]) == -1)
        std::cerr << "mPipe[1] close: " << strerror (errno) << std::endl;
}

int
ps::Pipe::ReadEnd () const
{
    return mPipe[0];
}

int &
ps::Pipe::WriteEnd ()
{
    return mPipe [1];
}
