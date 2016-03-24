/* /src/locatebinary.cpp
 *
 * Traverse PATH until a suitable instance
 * of a binary can be located
 *
 * See /LICENCE.md for Copyright information */

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream> // IWYU pragma: keep

#include <string.h>
#include <errno.h>  // IWYU pragma: keep
#include <unistd.h> // IWYU pragma: keep

#include <cpp-subprocess/locatebinary.h>
#include <cpp-subprocess/operating_system.h>


namespace ps = polysquare::subprocess;
namespace psi = polysquare::subprocess::impl;

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
    std::cerr << "Failed to locate file: " << e.what () << std::endl;
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
                throw ps::FileNotFoundError ();
            default:
                throw std::runtime_error (strerror (errno));
        }
    }

    return binary;
}
