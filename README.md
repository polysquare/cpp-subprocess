CPP Subprocess
==============
Some C++ subprocess handling classes.

Status
======
[![Build](https://travis-ci.org/polysquare/cpp-subprocess.svg?branch=master)](https://travis-ci.org/polysquare/cpp-subprocess)
[![Coverage](https://coveralls.io/repos/polysquare/cpp-subprocess/badge.png?branch=master)](https://coveralls.io/r/polysquare/cpp-subprocess?branch=master)

Usage
====

Finding a binary in $PATH
-------------------------

    #include <string>
    #include <vector>

    #include <boost/algorithm/string.hpp>

    #include <cpp-subprocess/locatebinary.h>
    #include <cpp-subprocess/operating_system.h>

    #include <cstdlib>

    namespace ps = polysquare::subprocess;

    char *paths = ::getenv ("PATH");

    std::vector <std::string> paths;
    boost::split (paths, boost::is_any_of (";"), boost::token_compress_on);

    std::string binary;

    try
    {
        binary = ps::locateBinary ("echo", paths, ps::MakeOperatingSystem ());
    }
    catch (std::runtime_error const &error)
    {
        std::cerr << error.what ();
    }


Launching a new binary asynchronously and capturing its input
-------------------------------------------------------------

    #include <cpp-subprocess/launch.h>
    #include <cpp-subprocess/operating_system.h>

    namespace ps = polysquare::subprocess;

    int stderr[2], stdout[2];

    pipe (stderr);
    pipe (stdout);

    pid_t pid = 0;
    try
    {
        char const **argv = {
            "hello",
            nullptr
        };
        pid = ps::launchBinary ("/usr/bin/echo"
                                argv,
                                environ,
                                stderr[1],
                                stdout[1],
                                ps::MakeOperatingSystem ());

    }
    catch (std::exception const &error)
    {
        std::cerr << error.what ();
    };

`stderr[0]` and `stdout[0]` can be used with `read` to read the output of the subprocess.

Waiting for an asynchronously launched binary to finish
-----------------------------------------------

    #include <cpp-subprocess/launch.h>

    try
    {
        ps::waitForProcessEnd (pid, ps::MakeOperatingSystem ());
    }
    catch (std::runtime_error const &error)
    {
        std::cerr << error.what ();
    }

Launching a subprocess synchronously
---------------------------------

    #include <cpp-subprocess/launch.h>
    #include <cpp-subprocess/operating_system.h>

    namespace ps = polysquare::subprocess;

    int stderr[2], stdout[2];

    pipe (stderr);
    pipe (stdout);

    pid_t pid = 0;
    try
    {
        char const **argv = {
            "hello",
            nullptr
        };
        pid = ps::launchBinaryAndWaitForReturn ("/usr/bin/echo"
                                                argv,
                                                environ,
                                                stderr[1],
                                                stdout[1],
                                                ps::MakeOperatingSystem ());

    }
    catch (std::exception const &error)
    {
        std::cerr << error.what ();
    };

Reading a file descriptor to a std::vector <string>
--------------------------------------------

    #include <cpp-subprocess/readfd.h>

    try
    {
        std::vector <string> lines = ps::ReadFDToLines (fd, ps::MakeOperatingSystem ());
    }

