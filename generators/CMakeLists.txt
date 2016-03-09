# /generators/CMakeLists.txt
# Build some simple code generators used to determine the
# available instrumentation tools at compile time
#
# See LICENCE.md for Copyright information

set (CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY
     cpp_subprocess_tools_generators_common)
set (CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/generators_common.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/generators_common.h)

psq_add_library (${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY} STATIC
                 SOURCES
                 ${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY_SRCS}
                 LIBRARIES ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
                 EXTERNAL_INCLUDE_DIRS
                 ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS})

set (CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/cpp_subprocess_string_replacement_generator.cpp)

psq_add_executable (${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR}
                    SOURCES
                    ${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR_SRCS}
                    LIBRARIES
                    ${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY}
                    EXTERNAL_INCLUDE_DIRS
                    ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS})