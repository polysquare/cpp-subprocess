/*
 * locatebinary.h
 *
 * Traverse PATH until a suitable instance
 * of a binary can be located
 *
 * See LICENSE.md for Copyright information
 */

#ifndef POLYSQUARE_CPP_SUBPROCESS_LOCATE_BINARY_H
#define POLYSQUARE_CPP_SUBPROCESS_LOCATE_BINARY_H

#include <exception>
#include <functional>  // IWYU pragma: keep
#include <stdexcept>
#include <string>

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem;

        namespace impl
        {
            std::string locateBinary (std::string const &binaryName,
                                      std::string const &path,
                                      OperatingSystem const &);

            typedef std::function <void (std::string const &)> ReportHandler;
            typedef std::function <void (ReportHandler const &)> PathReporter;
            std::string locationError (std::string const &binaryName,
                                       PathReporter const &reporter);
            void reportRuntimeError (std::runtime_error const &r);

        }

        class FileNotFoundError :
            public std::exception
        {
        };

        template <typename Container>
        std::string
        locateBinary (std::string         const &binaryName,
                      Container           const &searchPaths,
                      OperatingSystem     const &os)
        {
            for (std::string const &path : searchPaths)
            {
                try
                {
                    std::string binary (impl::locateBinary (binaryName,
                                                            path,
                                                            os));
                    return binary;
                }
                catch (FileNotFoundError const &f)
                {
                    continue;
                }
                catch (std::runtime_error const &r)
                {
                    impl::reportRuntimeError (r);
                }
            }

            auto reportFunc =
                [&searchPaths](impl::ReportHandler const &report) {
                    for (auto const &path : searchPaths)
                    {
                        report (path);
                    }
                };

            auto error (impl::locationError (binaryName,
                                             reportFunc));

            throw std::runtime_error (error);
        }

        std::string locateBinary (std::string     const &binaryName,
                                  OperatingSystem const &os);
    }
}

#endif
