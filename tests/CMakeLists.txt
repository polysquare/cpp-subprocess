# /tests/CMakeLists.txt
#
# Build acceptance and integration tests.
#
# See /LICENCE.md for Copyright information

set (CPP_SUBPROCESS_ACCEPTANCE_TESTS_DIRECTORY
     "${CMAKE_CURRENT_SOURCE_DIR}/acceptance")
set (CPP_SUBPROCESS_INTEGRATION_TESTS_DIRECTORY
     "${CMAKE_CURRENT_SOURCE_DIR}/integration")

psq_add_test_main (cpp_subprocess_test_main
                   SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_subdirectory ("${CPP_SUBPROCESS_ACCEPTANCE_TESTS_DIRECTORY}")
add_subdirectory ("${CPP_SUBPROCESS_INTEGRATION_TESTS_DIRECTORY}")
