/* /include/cpp-subprocess/fdbackup.h
 *
 * Utility class to save file descriptor names and restore them
 * later
 *
 * See /LICENCE.md for Copyright information */

#ifndef POLYSQUARE_CPP_SUBPROCESS_FDBACKUP_H
#define POLYSQUARE_CPP_SUBPROCESS_FDBACKUP_H

namespace polysquare
{
    namespace subprocess
    {
        class OperatingSystem;

        class FDBackup
        {
            public:

                FDBackup (int                   fd,
                          OperatingSystem const &os);
                ~FDBackup ();

            private:

                FDBackup (FDBackup const &) = delete;
                FDBackup (FDBackup &&) = delete;
                FDBackup & operator= (FDBackup const &) = delete;

                int mOriginalFd;
                int mBackupFd;

                OperatingSystem const &mOS;
        };
    }
}

#endif
