/* /src/readfd.cpp
 *
 * Some utility functions to read the contents of a file
 * descriptor, either into a vector of lines or
 * a single string.
 *
 * See /LICENCE.md for Copyright information */

#include <functional> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <sstream>
#include <vector>

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp> // IWYU pragma: keep

#include <fcntl.h> // IWYU pragma: keep

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/readfd.h> // IWYU pragma: keep

// IWYU pragma: no_include <__functional_03>
// IWYU pragma: no_include <boost/iostreams/categories.hpp>
// IWYU pragma: no_include <boost/iostreams/detail/error.hpp>
// IWYU pragma: no_include <boost/iostreams/detail/forward.hpp>
// IWYU pragma: no_include <boost/iostreams/stream_buffer.hpp>
// IWYU pragma: no_include <boost/iostreams/traits.hpp>
// IWYU pragma: no_include <bits/fcntl-linux.h>

namespace bio = boost::iostreams;
namespace ps = polysquare::subprocess;

namespace
{
    typedef std::function <void (std::string const &line,
                                 bool              newLine)> InsertLine;

    class NonblockGuard
    {
        public:

            NonblockGuard (int                       fd,
                           ps::OperatingSystem const &os) :
                mFD (fd),
                mOS (os),
                mLastFlags (os.fcntl_getfl (fd))
            {
                mOS.fcntl_setfl (mFD, mLastFlags | O_NONBLOCK);
            }

            ~NonblockGuard ()
            {
                mOS.fcntl_setfl (mFD, mLastFlags);
            }

        private:

            NonblockGuard (NonblockGuard const &) = delete;
            NonblockGuard (NonblockGuard &&) = delete;
            NonblockGuard & operator= (NonblockGuard const &) = delete;

            int                       mFD;
            ps::OperatingSystem const &mOS;
            int                       mLastFlags;
    };

    void ReadFD (int                       fd,
                 InsertLine const          &insert,
                 ps::OperatingSystem const &os)
    {
        NonblockGuard nonblock (fd, os);

        typedef bio::stream_buffer <bio::file_descriptor_source>  ChildStream;
        ChildStream streambuf (fd, bio::never_close_handle);

        std::istream stream (&streambuf);
        while (stream)
        {
            std::string line;
            std::getline (stream, line);

            /* If the stream "failed" it means we most likely got
             * -EWOULDBLOCK and this would indicate the end of
             * the stream. Don't insert newlines in this case */
            if (!line.empty ())
                insert (line, !stream.fail ());
        }
    }
}

std::vector <std::string>
ps::ReadFDToLines (int                       fd,
                   ps::OperatingSystem const &os)
{
    std::vector <std::string> lines;

    ReadFD (fd,
            [&lines](std::string const &line, bool n) -> void {
                lines.push_back (line);
            },
            os);

    return lines;
}

std::string
ps::ReadFDToString (int                       fd,
                    ps::OperatingSystem const &os)
{
    std::stringstream ss;

    ReadFD (fd,
            [&ss](std::string const &line, bool n) -> void {
                ss << line;
                if (n)
                    ss << std::endl;
            },
            os);

    return ss.str ();
}
