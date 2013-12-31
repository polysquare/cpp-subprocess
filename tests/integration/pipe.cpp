/*
 * pipe.cpp
 *
 * Test pipe creation and deletion with pipe2
 *
 * See LICENSE.md for Copyright information
 */

#include <stdexcept>

#include <gtest/gtest.h>

#include <unistd.h>
#include <sys/poll.h>
#include <errno.h>

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>

namespace ps = polysquare::subprocess;

class Pipe :
    public ::testing::Test
{
    public:

        Pipe ();

    protected:

        ps::OperatingSystem::Unique os;

        ps::Pipe pipe;
};

Pipe::Pipe () :
    os (ps::MakeOperatingSystem ()),
    pipe (*os)
{
}

TEST_F (Pipe, WriteAndRead)
{
    char msg[2] = "a";
    ssize_t amountWritten = os->write (pipe.WriteEnd (),
                                       static_cast <void *> (msg),
                                       1);

    if (amountWritten == -1)
        throw std::runtime_error (strerror (errno));

    char buf[2];

    struct pollfd pfd;
    pfd.events = POLLIN | POLLOUT | POLLHUP;
    pfd.revents = 0;
    pfd.fd = pipe.ReadEnd ();

    int ready = os->poll (&pfd, 1, 0);

    if (ready == -1)
        throw std::runtime_error (strerror (errno));

    ASSERT_EQ (1, ready);

    ssize_t amountRead = os->read (pipe.ReadEnd (),
                                   static_cast <void *> (buf),
                                   1);

    if (amountRead == -1)
        throw std::runtime_error (strerror (errno));

    EXPECT_EQ (msg[0], buf[0]);
}
