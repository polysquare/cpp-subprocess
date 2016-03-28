/* /include/cpp-subprocess/pipe.h
 *
 * Utility class to create a unix pipe and
 * close it on teardown
 *
 * See /LICENCE.md for Copyright information */

#ifndef POLYSQUARE_CPP_SUBPROCESS_PIPE_H
#define POLYSQUARE_CPP_SUBPROCESS_PIPE_H

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem;

        class Pipe
        {
            public:

                explicit Pipe (OperatingSystem const &os);
                ~Pipe ();

                /* Read end descriptor is read-only */
                int ReadEnd () const;

                /* Write end descriptor is writable, we need to close it
                 * from other objects */
                int & WriteEnd ();

            private:

                Pipe (Pipe const &) = delete;
                Pipe (Pipe &&) = delete;
                Pipe & operator= (Pipe const &) = delete;

                int mPipe[2];
                OperatingSystem const &mOS;
        };
    }
}

#endif
