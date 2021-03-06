# /tests/acceptance/CMakeLists.txt
#
# Build cpp-subprocess acceptance tests.
#
# See /LICENCE.md for Copyright information

set (SIMPLE_EXECUTABLE_BINARY simple_executable)
set (SIMPLE_EXECUTABLE_BINARY_DIRECTORY
     "${CMAKE_CURRENT_BINARY_DIR}/simple_executable")
set (ACCEPTANCE_TESTS_CONFIG_HEADER_NAME acceptance_tests_config.h)
set (ACCEPTANCE_TESTS_CONFIG_H
     "${CMAKE_CURRENT_BINARY_DIR}/${ACCEPTANCE_TESTS_CONFIG_HEADER_NAME}")
set (ACCEPTANCE_TESTS_CONFIG_H_IN
     "${CMAKE_CURRENT_SOURCE_DIR}/${ACCEPTANCE_TESTS_CONFIG_HEADER_NAME}.in")

set (REPLACEMENT_GENERATOR ${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR})
add_custom_command (OUTPUT ${ACCEPTANCE_TESTS_CONFIG_H}
                    COMMAND $<TARGET_FILE:${REPLACEMENT_GENERATOR}>
                    ARGS
                    -i
                    ${ACCEPTANCE_TESTS_CONFIG_H_IN}
                    -o
                    ${ACCEPTANCE_TESTS_CONFIG_H}
                    SIMPLE_EXECUTABLE_BINARY
                    ${SIMPLE_EXECUTABLE_BINARY}
                    SIMPLE_EXECUTABLE_BINARY_DIRECTORY
                    $<TARGET_FILE_DIR:${SIMPLE_EXECUTABLE_BINARY}>
                    DEPENDS
                    ${REPLACEMENT_GENERATOR}
                    ${ACCEPTANCE_TESTS_CONFIG_H_IN}
                    COMMENT "Generating acceptance tests configuration")


set (CPP_SUBPROCESS_ACCEPTANCE_TESTS
     cpp_subprocess_acceptance_tests)
set (CPP_SUBPROCESS_ACCEPTANCE_TESTS_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/launch.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/locatebinary.cpp"
     ${ACCEPTANCE_TESTS_CONFIG_H})

psq_add_test (${CPP_SUBPROCESS_ACCEPTANCE_TESTS}
              SOURCES ${CPP_SUBPROCESS_ACCEPTANCE_TESTS_SRCS}
              EXTERNAL_INCLUDE_DIRS
              ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
              INTERNAL_INCLUDE_DIRS
              "${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY}"
              "${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}"
              "${CPP_SUBPROCESS_MOCKS_DIRECTORY}"
              "${CMAKE_CURRENT_SOURCE_DIR}"
              "${CMAKE_CURRENT_BINARY_DIR}"
              LIBRARIES
              ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
              ${CPP_SUBPROCESS_LIBRARY}
              MOCKS ${CPP_SUBPROCESS_MOCKS}
              MAIN_LIBRARY "cpp_subprocess_test_main")


set (SIMPLE_EXECUTABLE_BINARY_SOURCE_DIRECTORY
     "${CMAKE_CURRENT_SOURCE_DIR}/simple_executable")
add_subdirectory ("${SIMPLE_EXECUTABLE_BINARY_SOURCE_DIRECTORY}")
