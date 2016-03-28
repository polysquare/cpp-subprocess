/* /tests/integration/pipe.cpp
 *
 * Test pipe creation and deletion with pipe
 *
 * See /LICENCE.md for Copyright information */

#include <sstream>
#include <stdexcept>
#include <string>

#include <poll.h>
#include <unistd.h>

#include <gtest/gtest.h>
#include <gmock/gmock-actions.h>
#include <gmock/gmock-generated-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-actions.h>
#include <gmock/gmock-spec-builders.h>

#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h>  // IWYU pragma: keep
#include <cpp-subprocess/redirectedfd.h>

#include <mock_operating_system.h>

namespace ps = polysquare::subprocess;
namespace psm = polysquare::subprocess::mocks;

using ::testing::_;
using ::testing::DoAll;
using ::testing::ElementsAreArray;
using ::testing::HasSubstr;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::SetArrayArgument;

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

    ASSERT_NE (amountWritten, -1) << "Expected data to be written";

    char buf[2];

    struct pollfd pfd;
    pfd.events = POLLIN | POLLOUT | POLLHUP;
    pfd.revents = 0;
    pfd.fd = pipe.ReadEnd ();

    int ready = os->poll (&pfd, 1, 0);

    ASSERT_EQ (1, ready) << "Expected read end to be ready";

    ssize_t amountRead = os->read (pipe.ReadEnd (),
                                   static_cast <void *> (buf),
                                   1);

    ASSERT_NE (amountRead, -1) << "Expected data to be read";
    EXPECT_EQ (msg[0], buf[0]) << "Expected data on read end";
}

class PipeWithMockedBackend :
    public ::testing::Test
{
    public:

        PipeWithMockedBackend ();

    protected:

        psm::OperatingSystem os;
};

PipeWithMockedBackend::PipeWithMockedBackend ()
{
    os.IgnoreAllCalls ();
}

TEST_F (PipeWithMockedBackend, ThrowsOnPipeFailure)
{
    ON_CALL (os, pipe (_)).WillByDefault (Return (-1));

    EXPECT_THROW ({
        ps::Pipe pipe (os);
    }, std::runtime_error);
}

TEST_F (PipeWithMockedBackend, ComplainsOnPipeFailureToClose)
{
    /* Redirect stderr so that we can capture it */
    ps::OperatingSystem::Unique realOS (ps::MakeOperatingSystem ());
    ps::Pipe                    stderrPipe (*realOS);
    ps::RedirectedFD            redirectedErrorsHandle (STDERR_FILENO,
                                                        stderrPipe.WriteEnd (),
                                                        *realOS);

    int returnedFDs[2] = { 10, 11 };

    ON_CALL (os, pipe (_)).
        WillByDefault (DoAll (SetArrayArgument <0> (returnedFDs,
                                                    returnedFDs + 2),
                       Return (0)));
    ON_CALL (os, close (returnedFDs[0])).WillByDefault (Return (-1));
    ON_CALL (os, close (returnedFDs[1])).WillByDefault (Return (-1));

    {
        ps::Pipe pipe (os);
    }

    auto lines = ps::ReadFDToLines (stderrPipe.ReadEnd (), *realOS);

    Matcher <std::string> const closeErrors[] =
    {
        HasSubstr ("mPipe[0] close:"),
        HasSubstr ("mPipe[1] close:")
    };

    EXPECT_THAT (lines, ElementsAreArray (closeErrors));
}
