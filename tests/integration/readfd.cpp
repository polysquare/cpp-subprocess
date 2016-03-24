/* /tests/integration/readfd.cpp
 *
 * Test reading from an arbitrary fd.
 *
 * See /LICENCE.md for Copyright information */

#include <string>
#include <system_error> // IWYU pragma: keep

#include <unistd.h>

#include <errno.h>  // IWYU pragma: keep

#include <gtest/gtest.h>
#include <gmock/gmock-generated-matchers.h>
#include <gmock/gmock-matchers.h>

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h> // IWYU pragma: keep

namespace ps = polysquare::subprocess;

using ::testing::ElementsAre;
using ::testing::StrEq;

class ReadFD :
    public ::testing::Test
{
    public:

        ReadFD ();

    protected:

        void WriteMessage (std::string const &msg);

        ps::OperatingSystem::Unique os;

        ps::Pipe pipe;
};

ReadFD::ReadFD () :
    os (ps::MakeOperatingSystem ()),
    pipe (*os)
{
}

void
ReadFD::WriteMessage (std::string const &msg)
{
    void const *cdata = static_cast <void const *> (msg.c_str ());
    void *data = const_cast <void *> (cdata);

    ssize_t amountWritten = os->write (pipe.WriteEnd (),
                                       data,
                                       msg.size ());

    if (amountWritten == -1)
        throw std::system_error (errno, std::system_category ());
}

TEST_F (ReadFD, ReadFromPipe)
{
    std::string const msg ("mock_data\n");
    WriteMessage (msg);

    std::string msgReceived = ps::ReadFDToString (pipe.ReadEnd (), os);

    EXPECT_EQ (msg, msgReceived);
}

TEST_F (ReadFD, ReadMultilineMessageFromPipe)
{
    std::string const msg ("mock_data\nmock_data\n");
    WriteMessage (msg);

    std::string msgReceived = ps::ReadFDToString (pipe.ReadEnd (), os);

    EXPECT_EQ (msg, msgReceived);
}

TEST_F (ReadFD, ReadMultilineMessageFromPipeNoTrailingReturn)
{
    std::string const msg ("mock_data\nmock_data");
    WriteMessage (msg);

    std::string msgReceived = ps::ReadFDToString (pipe.ReadEnd (), os);

    EXPECT_EQ (msg, msgReceived);
}

TEST_F (ReadFD, ReadMultilineMessageFromPipeAsVectorWithTrailingReturn)
{
    std::string const mockData ("mock_data");
    std::string const msg (mockData + "\n" + mockData + "\n");
    WriteMessage (msg);

    auto linesReceived = ps::ReadFDToLines (pipe.ReadEnd (), os);

    EXPECT_THAT (linesReceived,
                 ElementsAre (StrEq (mockData), StrEq (mockData)));
}

TEST_F (ReadFD, ReadMultilineMessageFromPipeAsVectorWithoutTrailingReturn)
{
    std::string const mockData ("mock_data");
    std::string const msg (mockData + "\n" + mockData);
    WriteMessage (msg);

    auto linesReceived = ps::ReadFDToLines (pipe.ReadEnd (), os);

    EXPECT_THAT (linesReceived,
                 ElementsAre (StrEq (mockData), StrEq (mockData)));
}

TEST_F (ReadFD, ReadNone)
{
    std::string msgReceived = ps::ReadFDToString (pipe.ReadEnd (), os);

    EXPECT_EQ ("", msgReceived);
}
