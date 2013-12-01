/*
 * readfd.cpp
 *
 * Test reading from an arbitrary fd.
 *
 * See LICENSE.md for Copyright information
 */

#include <stdexcept>

#include <gtest/gtest.h>

#include <unistd.h>
#include <errno.h>

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h>

namespace ps = polysquare::subprocess;

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
        throw std::runtime_error (strerror (errno));
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

TEST_F (ReadFD, ReadNone)
{
    std::string msgReceived = ps::ReadFDToString (pipe.ReadEnd (), os);

    EXPECT_EQ ("", msgReceived);
}
