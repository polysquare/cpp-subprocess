# /mocks/CMakeLists.txt
#
# Build polysquare_cpp_subprocess_mocks library.
#
# See /LICENCE.md for Copyright information

set (CPP_SUBPROCESS_MOCKS_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/mock_operating_system.cpp")

psq_add_mock (${CPP_SUBPROCESS_MOCKS}
              SOURCES ${CPP_SUBPROCESS_MOCKS_SRCS}
              EXTERNAL_INCLUDE_DIRS
              ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
              INTERNAL_INCLUDE_DIRS
              "${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}"
              "${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY}"
              LIBRARIES
              ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
              NO_CLANG_TIDY)
