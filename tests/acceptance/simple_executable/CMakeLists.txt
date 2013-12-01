# /tests/acceptance/simple_executable/CMakeLists.txt
#
# Build a simple executable that we can use for
# the acceptance tests
#
# See LICENCE.md for Copyright Information.

set (SIMPLE_EXECUTABLE_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/simple_executable.cpp)

add_executable (${SIMPLE_EXECUTABLE_BINARY}
                ${SIMPLE_EXECUTABLE_SRCS})

verapp_profile_check_source_files_conformance (${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${SIMPLE_EXECUTABLE_BINARY}
                                               ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${SIMPLE_EXECUTABLE_BINARY}
                         INCLUDES
                         ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIR}
                         ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIR})