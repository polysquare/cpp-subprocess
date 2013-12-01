/*
 * readfd.h
 *
 * Some utility functons to read the contents of a file
 * descriptor, either into a vector of lines or
 * a single string.
 *
 * See LICENCE.md for Copyright information.
 */

#ifndef POLYSQUARE_CPP_SUBPROCESS_READFD_H
#define POLYSQUARE_CPP_SUBPROCESS_READFD_H

#include <memory>
#include <vector>
#include <string>

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem;

        std::vector <std::string>
        ReadFDToLines (int                                     fd,
                       std::unique_ptr <OperatingSystem> const &os);

        std::string
        ReadFDToString (int                                     fd,
                        std::unique_ptr <OperatingSystem> const &os);
    }
}

#endif
