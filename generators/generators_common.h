/*
 * generators_common.h
 *
 * Tool used to parse an input template .h and .cpp files
 * and replace tokens for output files based on parameters.
 *
 * See LICENCE.md for Copyright information.
 */
#ifndef CPP_SUBPROCESS_GENERATORS_COMMON_H
#define CPP_SUBPROCESS_GENERATORS_COMMON_H

#include <functional>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace polysquare
{
    namespace subprocess
    {
        namespace generators
        {
            void
            GetOptions (int                       argc,
                        char                      **argv,
                        std::string               &input,
                        std::string               &output,
                        std::vector <std::string> &extra);

            std::string
            ReadInputFile (std::string const &input);

            void
            WriteToOutputFile (std::string const &output,
                               std::string const &data);

            template <typename CurrentTransform>
            void
            ApplyTransformations (std::string            &data,
                                  CurrentTransform const &transform)
            {
                transform (data);
            }

            template <typename CurrentTransform, typename... Transformations>
            void
            ApplyTransformations (std::string            &data,
                                  CurrentTransform const &transform,
                                  Transformations...     transformations)
            {
                transform (data);
                ApplyTransformations (data, transformations...);
            }

            namespace transformations
            {
                void DeleteComments (std::string &str);
                void CompressNewlines (std::string &str);
            }
        }
    }
}

#endif
