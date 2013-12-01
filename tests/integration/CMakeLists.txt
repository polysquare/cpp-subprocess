# /tests/integration/CMakeLists.txt
# Build cpp-subprocess integration tests.
#
# See LICENCE.md for Copyright information.

include_directories (${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY}
                     ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}
                     ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
                     ${CPP_SUBPROCESS_TEST_INCLUDE_DIRS}
                     ${CMAKE_CURRENT_SOURCE_DIR})

set (CPP_SUBPROCESS_INTEGRATION_TESTS
     cpp_subprocess_integration_tests)
set (CPP_SUBPROCESS_INTEGRATION_TESTS_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/fdbackup.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/pipe.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/readfd.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/redirectedfd.cpp)

add_executable (${CPP_SUBPROCESS_INTEGRATION_TESTS}
                ${CPP_SUBPROCESS_INTEGRATION_TESTS_SRCS})

target_link_libraries (${CPP_SUBPROCESS_INTEGRATION_TESTS}
                       ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
                       ${CPP_SUBPROCESS_TEST_LIBRARIES}
                       ${CPP_SUBPROCESS_LIBRARY})

verapp_profile_check_source_files_conformance (${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${CPP_SUBPROCESS_INTEGRATION_TESTS}
                                               ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${CPP_SUBPROCESS_INTEGRATION_TESTS}
                         INCLUDES
                         ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIR}
                         ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIR})