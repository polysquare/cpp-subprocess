/*
 * fdbackup.cpp
 *
 * Test backup and restore with dup2
 *
 * See LICENSE.md for Copyright information
 */

#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>

#include <unistd.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <errno.h>

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/fdbackup.h>

namespace ps = polysquare::subprocess;

class FDBackup :
    public ::testing::Test
{
    public:

        FDBackup ();

    protected:

        ps::OperatingSystem::Unique    os;
        int                            pipe[2];
        std::unique_ptr <ps::FDBackup> backup;
};

FDBackup::FDBackup () :
    os (ps::MakeOperatingSystem ())
{
    if (os->pipe (pipe) == -1)
        throw std::runtime_error (strerror (errno));

    backup.reset (new ps::FDBackup (pipe[0], *os));

    close (pipe[0]);
}

TEST_F (FDBackup, Restore)
{
    char msg[2] = "a";

    ssize_t amountWritten = os->write (pipe[1],
                                       static_cast <void *> (msg),
                                       1);

    if (amountWritten == -1)
        throw std::runtime_error (strerror (errno));

    backup.reset ();

    struct pollfd pfd;
    pfd.events = POLLIN | POLLOUT | POLLHUP;
    pfd.revents = 0;
    pfd.fd = pipe[0];

    int ready = os->poll (&pfd, 1, 0);

    if (ready == -1)
        throw std::runtime_error (strerror (errno));

    EXPECT_EQ (1, ready) << "Expected data available to be read on "
                            "restored fd";
}
