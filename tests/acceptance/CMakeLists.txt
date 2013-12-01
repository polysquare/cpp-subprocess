# /tests/acceptance/CMakeLists.txt
#
# Build cpp-subprocess acceptance tests
#
# See LICENCE.md for Copyright information.

set (SIMPLE_EXECUTABLE_BINARY simple_executable)
set (SIMPLE_EXECUTABLE_BINARY_DIRECTORY
     ${CMAKE_CURRENT_BINARY_DIR}/simple_executable)
set (ACCEPTANCE_TESTS_CONFIG_HEADER_NAME acceptance_tests_config.h)
set (ACCEPTANCE_TESTS_CONFIG_H
     ${CMAKE_CURRENT_BINARY_DIR}/${ACCEPTANCE_TESTS_CONFIG_HEADER_NAME})
set (ACCEPTANCE_TESTS_CONFIG_H_IN
     ${CMAKE_CURRENT_SOURCE_DIR}/${ACCEPTANCE_TESTS_CONFIG_HEADER_NAME}.in)

configure_file (${ACCEPTANCE_TESTS_CONFIG_H_IN}
                ${ACCEPTANCE_TESTS_CONFIG_H}
                @ONLY)

include_directories (${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY}
                     ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}
                     ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
                     ${CPP_SUBPROCESS_TEST_INCLUDE_DIRS}
                     ${CPP_SUBPROCESS_MOCKS_DIRECTORY}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR})

set (CPP_SUBPROCESS_ACCEPTANCE_TESTS
     cpp_subprocess_acceptance_tests)
set (CPP_SUBPROCESS_ACCEPTANCE_TESTS_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/launch.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/locatebinary.cpp)

add_executable (${CPP_SUBPROCESS_ACCEPTANCE_TESTS}
                ${CPP_SUBPROCESS_ACCEPTANCE_TESTS_SRCS})

target_link_libraries (${CPP_SUBPROCESS_ACCEPTANCE_TESTS}
                       ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
                       ${CPP_SUBPROCESS_TEST_LIBRARIES}
                       ${CPP_SUBPROCESS_LIBRARY}
                       ${CPP_SUBPROCESS_MOCKS})

verapp_profile_check_source_files_conformance (${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${CPP_SUBPROCESS_ACCEPTANCE_TESTS}
                                               ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${CPP_SUBPROCESS_ACCEPTANCE_TESTS}
                         INCLUDES
                         ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIR}
                         ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIR})

set (SIMPLE_EXECUTABLE_BINARY_SOURCE_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}/simple_executable)
add_subdirectory (${SIMPLE_EXECUTABLE_BINARY_SOURCE_DIRECTORY})
