# /include/cpp-subprocess/CMakeLists.txt
# Ensure that the cpp-subprocess headers are scanned for source code
# style compliance
#
# See LICENCE.md for Copyright information

set (CPP_SUBPROCESS_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/fdbackup.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/launch.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/locatebinary.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/operating_system.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/pipe.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/readfd.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/redirectedfd.h")

psq_add_checked_sources (cpp_subprocess_headers
                         SOURCES ${CPP_SUBPROCESS_HEADERS})