# /tests/acceptance/simple_executable/CMakeLists.txt
#
# Build a simple executable that we can use for
# the acceptance tests
#
# See LICENCE.md for Copyright Information.

set (SIMPLE_EXECUTABLE_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/simple_executable.cpp)

psq_add_executable (${SIMPLE_EXECUTABLE_BINARY}
                    SOURCES ${SIMPLE_EXECUTABLE_SRCS})