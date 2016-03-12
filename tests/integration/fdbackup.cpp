/*
 * fdbackup.cpp
 *
 * Test backup and restore with dup2
 *
 * See LICENSE.md for Copyright information
 */

#include <memory>  // IWYU pragma: keep
#include <stdexcept>  // IWYU pragma: keep
#include <string>  // IWYU pragma: keep
#include <system_error>  // IWYU pragma: keep

#include <poll.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>  // IWYU pragma: keep

#include <gmock/gmock-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest.h>

#include <cpp-subprocess/fdbackup.h>
#include <cpp-subprocess/operating_system.h>  // IWYU pragma: keep
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h>  // IWYU pragma: keep
#include <cpp-subprocess/redirectedfd.h>

#include <mock_operating_system.h>


namespace ps = polysquare::subprocess;
namespace psm = polysquare::subprocess::mocks;

using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;

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
    int result = os->pipe (pipe);

    assert (result != -1);

    backup.reset (new ps::FDBackup (pipe[0], *os));

    close (pipe[0]);
}

TEST_F (FDBackup, Restore)
{
    char msg[2] = "a";

    ssize_t amountWritten = os->write (pipe[1],
                                       static_cast <void *> (msg),
                                       1);

    assert (amountWritten != -1);

    backup.reset ();

    struct pollfd pfd;
    pfd.events = POLLIN | POLLOUT | POLLHUP;
    pfd.revents = 0;
    pfd.fd = pipe[0];

    int ready = os->poll (&pfd, 1, 0);

    assert (ready != -1);

    EXPECT_EQ (1, ready) << "Expected data available to be read on restored fd";
}

class MockedOSFDBackup :
    public ::testing::Test
{
    public:

        MockedOSFDBackup ();

    protected:

        psm::OperatingSystem os;
};

MockedOSFDBackup::MockedOSFDBackup ()
{
    os.IgnoreAllCalls ();
}

TEST_F (MockedOSFDBackup, ThrowRuntimeErrorOnDupFailure)
{
    int originalFd = 1;
    ON_CALL (os, dup (originalFd)).WillByDefault (Return (-1));

    EXPECT_THROW ({
        ps::FDBackup backup (originalFd, os);
    }, std::runtime_error);
}

TEST_F (MockedOSFDBackup, ComplainsToStandardErrorWhenDup2Fails)
{
    /* Redirect stderr so that we can capture it */
    ps::OperatingSystem::Unique realOS (ps::MakeOperatingSystem ());
    ps::Pipe                    stderrPipe (*realOS);
    ps::RedirectedFD            redirectedErrorsHandle (STDERR_FILENO,
                                                        stderrPipe.WriteEnd (),
                                                        *realOS);

    ON_CALL (os, dup (_)).WillByDefault (Return (1));
    ON_CALL (os, dup2 (_, _)).WillByDefault (Return (-1));

    {
        ps::FDBackup backup (1, os);
    }

    auto errors = ps::ReadFDToString (stderrPipe.ReadEnd (), realOS);

    EXPECT_THAT (errors, HasSubstr ("Failed to restore file descriptor"));
}
