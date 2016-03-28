/* /tests/acceptance/launch.cpp
 *
 * Test that when we launch a binary we can collect
 * its return code, output etc
 *
 * See /LICENCE.md for Copyright information */

#include <string> // IWYU pragma: keep

#include <gtest/gtest.h>

#include <stddef.h>

#include <cpp-subprocess/launch.h>
#include <cpp-subprocess/operating_system.h> // IWYU pragma: keep
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/readfd.h>  // IWYU pragma: keep

#include <acceptance_tests_config.h>

namespace ps = polysquare::subprocess;
namespace pst = polysquare::subprocess::test;

class Launch :
    public ::testing::Test
{
    public:

        Launch ();

    protected:

        int LaunchBinary ();

        std::string const executable;

        ps::OperatingSystem::Unique os;

        ps::Pipe         stderrPipe;
        ps::Pipe         stdoutPipe;
};

Launch::Launch () :
    executable (pst::SimpleExecutablePath),
    os (ps::MakeOperatingSystem ()),
    stderrPipe (*os),
    stdoutPipe (*os)
{
}

int
Launch::LaunchBinary ()
{
    char const *argv = NULL;
    char const *environment = NULL;
    return ps::launchBinaryAndWaitForReturn (executable,
                                             &argv,
                                             &environment,
                                             stderrPipe.WriteEnd (),
                                             stdoutPipe.WriteEnd (),
                                             *os);
}

TEST_F (Launch, CaptureStdout)
{
    LaunchBinary ();
    std::string output = ps::ReadFDToString (stdoutPipe.ReadEnd (), *os);
    EXPECT_EQ ("output\n", output);
}

TEST_F (Launch, CaptureStderr)
{
    LaunchBinary ();
    std::string error = ps::ReadFDToString (stderrPipe.ReadEnd (), *os);
    EXPECT_EQ ("error\n", error);
}

TEST_F (Launch, ReturnCode)
{
    int ret = LaunchBinary ();
    EXPECT_EQ (0, ret);
}
