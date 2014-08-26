/*
 * locatebinary.cpp
 *
 * Test traversing PATH until a suitable
 * instance of a binary can be located
 *
 * See LICENSE.md for Copyright information
 */

#include <array>
#include <functional>

#include <cstdlib>

#include <boost/tokenizer.hpp>

#include <gmock/gmock.h>

#include "acceptance_tests_config.h"

#include <cpp-subprocess/locatebinary.h>
#include <cpp-subprocess/pipe.h>
#include <cpp-subprocess/operating_system.h>
#include <cpp-subprocess/redirectedfd.h>
#include <cpp-subprocess/readfd.h>

#include <mock_operating_system.h>

using ::testing::_;
using ::testing::DoAll;
using ::testing::HasSubstr;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;

namespace ps = polysquare::subprocess;
namespace psm = polysquare::subprocess::mocks;
namespace pst = polysquare::subprocess::test;

namespace
{
    template <class T>
    T * ValueOrDefault (T *value, T *defaultValue)
    {
        if (value)
            return value;

        return defaultValue;
    }

    class TmpEnv
    {
        public:

            TmpEnv (char const *env, char const *value);
            ~TmpEnv ();

        private:

            TmpEnv (TmpEnv const &) = delete;
            TmpEnv (TmpEnv &&) = delete;
            TmpEnv & operator= (TmpEnv const &) = delete;

            std::string const mEnvironmentKey;
            std::string const mPrevEnvironmentValue;
    };

    /* Older versions of cppcheck get confused by
     * standalone strings as part of array initializers
     * so we use an array-assign here */
    std::array <std::string, 2> MockExecutablePaths =
    {
        {
            "/mock/to/path",
            "/path/to/mock"
        }
    };

    char const *MockExecutable = "mock_executable";

    template <typename T>
    std::array <T, 1>
    SingleArray (T const &t)
    {
        return std::array <T, 1> {
                                     {
                                         t
                                     }
                                 };
    }
    
    typedef char const * CharCP;
}

TmpEnv::TmpEnv (char const *env, char const *value) :
    mEnvironmentKey (env),
    mPrevEnvironmentValue (ValueOrDefault (const_cast <CharCP> (getenv (env)),
                                           ""))
{
    setenv (env, value, 1);
}

TmpEnv::~TmpEnv ()
{
    if (mPrevEnvironmentValue.empty ())
        unsetenv (mEnvironmentKey.c_str ());
    else
        setenv (mEnvironmentKey.c_str (),
                mPrevEnvironmentValue.c_str (),
                1);
}

class LocateBinary :
    public ::testing::Test
{
    public:

        LocateBinary ();

    protected:

        psm::OperatingSystem os;
};

LocateBinary::LocateBinary ()
{
    os.IgnoreAllCalls ();
}

ACTION_P (SimulateError, err)
{
    errno = err;
    return -1;
}

TEST_F (LocateBinary, ThrowRuntimeErrorOnEmptyPath)
{
    std::stringstream PassedPathBuilder;
    PassedPathBuilder << "/" << MockExecutable;
    ON_CALL (os, access (StrEq (PassedPathBuilder.str ()), _))
        .WillByDefault (SimulateError (ENOENT));

    EXPECT_THROW ({
        ps::locateBinary (MockExecutable,
                          SingleArray <char const *> (""),
                          os);
    }, std::runtime_error);
}

TEST_F (LocateBinary, ThrowRuntimeErrorOnNotFoundInOnePath)
{
    std::string const FullPath (MockExecutablePaths[0] + "/" + MockExecutable);
    ON_CALL (os, access (StrEq (FullPath), _))
        .WillByDefault (SimulateError (ENOENT));

    EXPECT_THROW ({
        ps::locateBinary (MockExecutable,
                          SingleArray (MockExecutablePaths[0]),
                          os);
    }, std::runtime_error);
}

TEST_F (LocateBinary, ThrownErrorSpecifiesFileInWhatString)
{
    std::string const FullPath (MockExecutablePaths[0] + "/" + MockExecutable);
    ON_CALL (os, access (StrEq (FullPath), _))
        .WillByDefault (SimulateError (ENOENT));

    try
    {
        ps::locateBinary (MockExecutable,
                          SingleArray (MockExecutablePaths[0]),
                          os);
    }
    catch (std::runtime_error &err)
    {
        EXPECT_THAT (err.what (), HasSubstr (MockExecutablePaths[0]));
    }
}

TEST_F (LocateBinary, ComplainToStderrOnOtherError)
{
    /* Redirect stderr so that we can capture it */
    ps::OperatingSystem::Unique realOS (ps::MakeOperatingSystem ());
    ps::Pipe                    stderrPipe (*realOS);
    ps::RedirectedFD            redirectedErrorsHandle (STDERR_FILENO,
                                                        stderrPipe.WriteEnd (),
                                                        *realOS);

    ON_CALL (os, access (_, _))
        .WillByDefault (SimulateError (EAGAIN));

    try
    {
        ps::locateBinary (MockExecutable,
                          SingleArray (MockExecutablePaths[0]),
                          os);
    }
    catch (std::runtime_error &)
    {
    }

    auto lines = ps::ReadFDToLines (stderrPipe.ReadEnd (), realOS);

    Matcher <std::string> const closeErrors[] =
    {
        HasSubstr ("Failed to locate file: ")
    };

    EXPECT_THAT (lines, ElementsAreArray (closeErrors));
}

TEST_F (LocateBinary, ThrowRuntimeErrorOnNotFoundInMultiplePaths)
{
    std::string const FullPath0 (MockExecutablePaths[0] + "/" + MockExecutable);
    std::string const FullPath1 (MockExecutablePaths[1] + "/" + MockExecutable);

    ON_CALL (os, access (StrEq (FullPath0), _))
        .WillByDefault (SimulateError (ENOENT));
    ON_CALL (os, access (StrEq (FullPath1), _))
        .WillByDefault (SimulateError (ENOENT));

    EXPECT_THROW ({
        ps::locateBinary (MockExecutable,
                          MockExecutablePaths,
                          os);
    }, std::runtime_error);
}

TEST_F (LocateBinary, ReturnFullyQualifiedPathIfFoundInOnePath)
{
    std::string const ExpectedPath (MockExecutablePaths[0] +
                                    "/" +
                                    MockExecutable);

    ON_CALL (os, access (StrEq (ExpectedPath), _))
        .WillByDefault (Return (0));

    EXPECT_EQ (ExpectedPath, ps::locateBinary (MockExecutable,
                                               MockExecutablePaths,
                                               os));
}

TEST_F (LocateBinary, ReturnFullyQualifiedPathIfFoundOtherPath)
{
    std::string const NoFindPath (MockExecutablePaths[0] +
                                  "/" +
                                  MockExecutable);
    std::string const ExpectedPath (MockExecutablePaths[1] +
                                    "/" +
                                    MockExecutable);

    ON_CALL (os, access (StrEq (NoFindPath), _))
        .WillByDefault (SimulateError (ENOENT));
    ON_CALL (os, access (StrEq (ExpectedPath), _))
        .WillByDefault (Return (0));

    /* We really should be checking twice */
    EXPECT_CALL (os, access (_, X_OK)).Times (2);

    EXPECT_EQ (ExpectedPath,
               ps::locateBinary (MockExecutable,
                                 MockExecutablePaths,
                                 os));
}

namespace
{
    std::string const NewPath ("/:" +
                               std::string (pst::SimpleExecutableDirectory) +
                               ":" +
                               getenv ("PATH"));
    std::string const NoExecutable ("______none______");
}

class LocateBinaryWithSystemPath :
    public ::testing::Test
{
    public:

        LocateBinaryWithSystemPath ();

    protected:

        ps::OperatingSystem::Unique os;

    private:

        TmpEnv path;
};

LocateBinaryWithSystemPath::LocateBinaryWithSystemPath () :
    os (ps::MakeOperatingSystem ()),
    path ("PATH", NewPath.c_str ())
{
}

TEST_F (LocateBinaryWithSystemPath, SimpleExecutable)
{
    std::string pathString (getenv ("PATH"));
    boost::char_separator <char> sep (":");
    boost::tokenizer <boost::char_separator <char> > paths (pathString, sep);

    EXPECT_THAT (ps::locateBinary (pst::SimpleExecutable,
                                   paths,
                                   *os),
                 StrEq (pst::SimpleExecutablePath));
}
