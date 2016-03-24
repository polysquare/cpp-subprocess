/* /tests/integration/redirectedfd.cpp
 *
 * Test redirecting one fd to another
 *
 * See /LICENCE.md for Copyright information */

#include <memory> // IWYU pragma: keep
#include <stdexcept>
#include <system_error> // IWYU pragma: keep

#include <poll.h>
#include <string.h>
#include <errno.h> // IWYU pragma: keep
#include <unistd.h>

#include <gmock/gmock-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>

#include <cpp-subprocess/fdbackup.h>
#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h> // IWYU pragma: keep
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

        if (n == -1)
            throw std::system_error (errno, std::system_category ());

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

    if (result == -1)
        throw std::system_error (errno, std::system_category ());

    redirected.reset (new ps::RedirectedFD (from.WriteEnd (),
                                            to.WriteEnd (),
                                            *os));
}

TEST_F (RedirectedFD, WriteToNewFd)
{
    char buf[] = "a\0";
    ssize_t len = write (from.WriteEnd (), static_cast <void *> (buf), 1);

    ASSERT_NE (len, -1) << strerror (errno);

    /* There should be something to poll */
    EXPECT_TRUE (ReadyForReading (to, *os));
}

TEST_F (RedirectedFD, RestoredWriteToOldFd)
{
    redirected.reset ();
    backup.reset ();

    char buf[] = "a\0";
    ssize_t len = write (from.WriteEnd (), static_cast <void *> (buf), 1);

    ASSERT_NE (len, -1) << strerror (errno);

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
