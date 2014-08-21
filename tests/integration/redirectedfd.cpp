/*
 * redirectedfd.cpp
 *
 * Test redirecting one fd to another
 *
 * See LICENSE.md for Copyright information
 */
#include <stdexcept>

#include <memory>

#include <gmock/gmock.h>

#include <stdio.h>
#include <sys/poll.h>

#include <cpp-subprocess/fdbackup.h>
#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h>
#include <cpp-subprocess/redirectedfd.h>

#include <mock_operating_system.h>

namespace ps = polysquare::subprocess;
namespace psm = polysquare::subprocess::mocks;

using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;

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

        assert (n != -1);

        return n > 0;
    }
}

RedirectedFD::RedirectedFD () :
    os (ps::MakeOperatingSystem ()),
    from (*os),
    to (*os),
    backup (new ps::FDBackup (from.WriteEnd (), *os))
{
    int result = close (from.WriteEnd ());

    assert (result != -1);

    redirected.reset (new ps::RedirectedFD (from.WriteEnd (),
                                            to.WriteEnd (),
                                            *os));
}

TEST_F (RedirectedFD, WriteToNewFd)
{
    char buf[] = "a\0";
    ssize_t len = write (from.WriteEnd (), static_cast <void *> (buf), 1);

    assert (len != -1);

    /* There should be something to poll */
    EXPECT_TRUE (ReadyForReading (to, *os));
}

TEST_F (RedirectedFD, RestoredWriteToOldFd)
{
    redirected.reset ();
    backup.reset ();

    char buf[] = "a\0";
    ssize_t len = write (from.WriteEnd (), static_cast <void *> (buf), 1);

    assert (len != -1);

    /* There should be nothing to poll */
    EXPECT_TRUE (ReadyForReading (from, *os));
}

class MockedOSRedirectedFD :
    public ::testing::Test
{
    public:

        MockedOSRedirectedFD ();

    protected:

        psm::OperatingSystem os;
};

MockedOSRedirectedFD::MockedOSRedirectedFD ()
{
    os.IgnoreAllCalls ();
}

TEST_F (MockedOSRedirectedFD, ThrowsRuntimeErrorOnFailureToCallDup)
{
    ON_CALL (os, dup2 (_, _)).WillByDefault (Return (-1));

    EXPECT_THROW ({
        int to;

        ps::RedirectedFD redirected (0, to, os);
    }, std::runtime_error);
}

TEST_F (MockedOSRedirectedFD, ComplainsToStandardErrorWhenCloseFails)
{
    /* Redirect stderr so that we can capture it */
    ps::OperatingSystem::Unique realOS (ps::MakeOperatingSystem ());
    ps::Pipe                    stderrPipe (*realOS);
    ps::RedirectedFD            redirectedErrorsHandle (STDERR_FILENO,
                                                        stderrPipe.WriteEnd (),
                                                        *realOS);

    ON_CALL (os, close (_)).WillByDefault (Return (-1));

    {
        int to = 1;
        ps::RedirectedFD redirected (0, to, os);
    }

    auto errors = ps::ReadFDToString (stderrPipe.ReadEnd (), realOS);

    EXPECT_THAT (errors, HasSubstr ("Failed to close redirect-to"));
}
