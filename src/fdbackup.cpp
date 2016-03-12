/*
 * fdbackup.cpp
 *
 * Utility class to save file descriptor names and restore them
 * later
 *
 * See LICENSE.md for Copyright information
 */

#include <iostream>
#include <stdexcept>

#include <errno.h>  // IWYU pragma: keep
#include <string.h>

#include <cpp-subprocess/fdbackup.h>
#include <cpp-subprocess/operating_system.h>

namespace ps = polysquare::subprocess;

ps::FDBackup::FDBackup (int                   fd,
                        OperatingSystem const &os) :
    mOriginalFd (fd),
    mBackupFd (0),
    mOS (os)
{
    mBackupFd = mOS.dup (mOriginalFd);

    /* Save original */
    if (mBackupFd == -1)
        throw std::runtime_error (strerror (errno));
}

ps::FDBackup::~FDBackup ()
{
    /* Redirect backed up fd to old fd location */
    if (mBackupFd &&
        mOS.dup2 (mBackupFd, mOriginalFd) == -1)
        std::cerr << "Failed to restore file descriptor "
                  << strerror (errno) << std::endl;
}
