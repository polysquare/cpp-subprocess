/*
 * redirectedfd.h
 *
 * Utility class to redirect operations on one
 * file descriptor into another until destruction.
 *
 * See LICENSE.md for Copyright information
 */
#ifndef POLYSQUARE_CPP_SUBPROCESS_REDIRECTEDFD_H
#define POLYSQUARE_CPP_SUBPROCESS_REDIRECTEDFD_H

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem;

        class RedirectedFD
        {
            public:

                RedirectedFD (int                   from,
                              int                   &to,
                              OperatingSystem const &os);
                ~RedirectedFD ();


            private:

                RedirectedFD (RedirectedFD const &) = delete;
                RedirectedFD (RedirectedFD &&) = delete;
                RedirectedFD & operator= (RedirectedFD const &) = delete;

                int &mToFd;
                OperatingSystem const &mOS;
        };
    }
}

#endif
