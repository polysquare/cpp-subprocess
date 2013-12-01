/*
 * redirectedfd.cpp
 *
 * Test redirecting one fd to another
 *
 * See LICENSE.md for Copyright information
 */
#include <stdexcept>

#include <memory>

#include <gtest/gtest.h>

#include <stdio.h>
#include <sys/poll.h>

#include <cpp-subprocess/fdbackup.h>
#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/redirectedfd.h>

namespace ps = polysquare::subprocess;

class RedirectedFD :
    public ::testing::Test
{
    public:

        RedirectedFD ();

    protected:

        ps::OperatingSystem::Unique os;

        ps::Pipe from;
        ps::Pipe to;
        std::unique_ptr <ps::FDBackup> backup;
        std::unique_ptr <ps::RedirectedFD> redirected;
};

namespace
{
    bool ReadyForReading (ps::Pipe            const &pipe,
                          ps::OperatingSystem const &os)
    {
        /* There should be nothing to poll */
        struct pollfd pfd;
        pfd.events = POLLIN | POLLOUT | POLLHUP;
        pfd.revents = 0;
        pfd.fd = pipe.ReadEnd ();

        int n = os.poll (&pfd, 1, 0);

        if (n == -1)
            throw std::runtime_error (strerror (errno));

        return n > 0;
    }
}

RedirectedFD::RedirectedFD () :
    os (ps::MakeOperatingSystem ()),
    from (*os),
    to (*os),
    backup (new ps::FDBackup (from.WriteEnd (), *os))
{
    if (close (from.WriteEnd ()) == -1)
        throw std::runtime_error (strerror (errno));

    redirected.reset (new ps::RedirectedFD (from.WriteEnd (),
                                            to.WriteEnd (),
                                            *os));
}

TEST_F (RedirectedFD, WriteToNewFd)
{
    char buf[] = "a\0";
    ssize_t len = write (from.WriteEnd (), static_cast <void *> (buf), 1);

    if (len == -1)
        throw std::runtime_error (strerror (errno));

    /* There should be something to poll */
    EXPECT_TRUE (ReadyForReading (to, *os))
        << "Expected data on to pipe";
}

TEST_F (RedirectedFD, RestoredWriteToOldFd)
{
    redirected.reset ();
    backup.reset ();

    char buf[] = "a\0";
    ssize_t len = write (from.WriteEnd (), static_cast <void *> (buf), 1);

    if (len == -1)
        throw std::runtime_error (strerror (errno));

    /* There should be nothing to poll */
    EXPECT_TRUE (ReadyForReading (from, *os))
        << "Expected data on from pipe";
}
