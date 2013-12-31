/*
 * locatebinary.cpp
 *
 * Traverse PATH until a suitable instance
 * of a binary can be located
 *
 * See LICENSE.md for Copyright information
 */

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <cpp-subprocess/locatebinary.h>
#include <cpp-subprocess/operating_system.h>

namespace ps = polysquare::subprocess;
namespace psi = polysquare::subprocess::impl;

ps::FileNotFoundError::FileNotFoundError (std::string const &file) :
    mFile (file)
{
}

ps::FileNotFoundError::~FileNotFoundError () noexcept (true)
{
}

char const *
ps::FileNotFoundError::what () const noexcept (true)
{
    return "A file was not found";
}

std::string
psi::locationError (std::string   const &binaryName,
                    PathReporter  const &reporter)
{
    std::stringstream ss;
    ss << "Could not find the executable " << binaryName
       << " anywhere in your search path " << std::endl
       << "Searched the following paths: " << std::endl;

    reporter ([&ss](std::string const &str) {
                  ss << str << std::endl;
              });
    return ss.str ();
}

void
psi::reportRuntimeError (std::runtime_error const &e)
{
    std::cout << e.what () << std::endl;
}

std::string
psi::locateBinary (std::string         const &binaryName,
                   std::string         const &path,
                   ps::OperatingSystem const &os)
{
    std::string binary (path + "/" + binaryName);
    int ok = os.access (binary.c_str (), X_OK);

    if (ok == -1)
    {
        switch (errno)
        {
            case EACCES:
            case ENOENT:
                throw ps::FileNotFoundError (binaryName);
            default:
                throw std::runtime_error (strerror (errno));
        }
    }

    return binary;
}
