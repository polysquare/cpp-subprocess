/*
 * mock_operating_system.cpp
 *
 * Implementation detals for Mock Operating System
 * calls for cpp-subprocess.
 *
 * See LICENCE.md for Copyright information.
 */

#include "mock_operating_system.h"

using ::testing::_;
using ::testing::AtLeast;

namespace psm = polysquare::subprocess::mocks;

psm::OperatingSystem::OperatingSystem ()
{
}

psm::OperatingSystem::~OperatingSystem ()
{
}

void
psm::OperatingSystem::IgnoreAllCalls ()
{
    EXPECT_CALL (*this, pipe (_)).Times (AtLeast (0));
    EXPECT_CALL (*this, execve (_, _, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, access (_, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, dup (_)).Times (AtLeast (0));
    EXPECT_CALL (*this, dup2 (_, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, close (_)).Times (AtLeast (0));
    EXPECT_CALL (*this, waitpid (_, _, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, fork ()).Times (AtLeast (0));
    EXPECT_CALL (*this, poll (_, _, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, read (_, _, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, write (_, _, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, fcntl_setfl (_, _)).Times (AtLeast (0));
    EXPECT_CALL (*this, fcntl_getfl (_)).Times (AtLeast (0));
}
