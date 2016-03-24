# /tests/integration/CMakeLists.txt
#
# Build cpp-subprocess integration tests.
#
# See /LICENCE.md for Copyright information

set (CPP_SUBPROCESS_INTEGRATION_TESTS
     cpp_subprocess_integration_tests)
set (CPP_SUBPROCESS_INTEGRATION_TESTS_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/fdbackup.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/pipe.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/readfd.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/redirectedfd.cpp")

psq_add_test (${CPP_SUBPROCESS_INTEGRATION_TESTS}
              SOURCES ${CPP_SUBPROCESS_INTEGRATION_TESTS_SRCS}
              EXTERNAL_INCLUDE_DIRS
              ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
              INTERNAL_INCLUDE_DIRS
              "${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY}"
              "${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}"
              "${CPP_SUBPROCESS_MOCKS_DIRECTORY}"
              "${CMAKE_CURRENT_SOURCE_DIR}"
              LIBRARIES
              ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
              ${CPP_SUBPROCESS_LIBRARY}
              MOCKS ${CPP_SUBPROCESS_MOCKS})
