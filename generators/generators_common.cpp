/* /generators/generators_common.cpp
 *
 * Tool used to parse an input template /.h and /.cpp files
 * and replace tokens for output files based on parameters.
 *
 * See /LICENCE.md for Copyright information */

#include <algorithm>
#include <exception>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include <cstring>

#include <boost/program_options.hpp>

#include "generators_common.h"

namespace po = boost::program_options;
namespace psg = polysquare::subprocess::generators;
namespace psgt = polysquare::subprocess::generators::transformations;

void
psg::GetOptions (int                       argc,
                 char                      **argv,
                 std::string               &input,
                 std::string               &output,
                 std::vector <std::string> &extra)
{
    po::options_description description ("Options");

    description.add_options ()
        ("input,i",
         po::value <std::string> (&input)->required (),
         "File to read template from")
        ("output,o",
         po::value <std::string> (&output)->required (),
         "File to write result to");

    po::variables_map vm;
    po::command_line_parser parser (argc, argv);

    po::parsed_options parsed (parser.options (description)
                                   .allow_unregistered ()
                                   .run ());

    po::store (parsed,
               vm);

    try
    {
        po::notify (vm);

        extra = po::collect_unrecognized (parsed.options,
                                          po::include_positional);
    }
    catch (std::exception const &e)
    {
        std::stringstream ss;

        ss << e.what () << std::endl;
        ss << "Options Available: " << description << std::endl;

        throw std::logic_error (ss.str ());
    }
}

std::string
psg::ReadInputFile (std::string const &input)
{
    std::fstream inputFile (input.c_str (),
                            std::ios_base::in);

    char inputFileBuffer[256];
    std::stringstream inputFileData;

    while (!inputFile.eof ())
    {
        if (inputFile.fail ())
        {
            std::stringstream ss;

            ss << "Failed to read file "
               << input;

            throw std::runtime_error (ss.str ());
        }

        inputFile.getline (inputFileBuffer, 256);
        inputFileData << inputFileBuffer << "\n";
    }

    return inputFileData.str ();
}

void
psg::WriteToOutputFile (std::string const &output,
                        std::string const &data)
{
    std::fstream outputFile (output.c_str (),
                             std::ios_base::out |
                             std::ios_base::trunc);

    outputFile << data;

    if (outputFile.fail ())
    {
        std::stringstream ss;
        ss << "Written file " << output << " may be inconsistent!";

        throw std::runtime_error (ss.str ());
    }
}

void
psgt::DeleteComments (std::string &str)
{
    size_t beginCommentPosition = str.find ("/*");

    while (beginCommentPosition != std::string::npos)
    {
        size_t endCommentPosition = str.find ("*/");

        if (endCommentPosition == std::string::npos)
            throw std::runtime_error ("Comment does not end, "
                                      "malformed file?");

        std::string::iterator beginCommentIterator (str.begin ());
        std::string::iterator endCommentIterator (str.begin ());

        std::advance (beginCommentIterator, beginCommentPosition);

        /* We need to advance 2 extra since std::string::find gives us
         * the position of the character where the match starts, but
         * we want to erase inclusive of the end */
        std::advance (endCommentIterator, endCommentPosition + 2);

        std::string::iterator erasureIterator =
            str.erase (beginCommentIterator, endCommentIterator);

        beginCommentPosition = std::distance (str.begin (),
                                              erasureIterator);

        beginCommentPosition = str.find ("/*", beginCommentPosition);
    }
}

void
psgt::CompressNewlines (std::string &str)
{
    size_t position = str.find ("\n");

    while (position != std::string::npos)
    {
        char const *cstr (str.c_str ());
        size_t compressEndPoint = position;

        while ((cstr + compressEndPoint)[0] == '\n')
        {
            ++compressEndPoint;

            if (compressEndPoint >= str.size ())
                compressEndPoint = str.size ();
        }

        size_t eraseDistance = compressEndPoint - position;

        /* Compress only if we are dealing with more than 2 \n's */
        if (eraseDistance > 2)
        {
            str.erase (position, compressEndPoint - position);
            str.insert (position, "\n\n");
        }

        position = str.find ("\n", position + 1);
    }
}
