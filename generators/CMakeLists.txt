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

include_directories(${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS})

add_library (${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY} STATIC
             ${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY_SRCS})

target_link_libraries (${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY}
                       ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES})

set (VERAPP_OUTPUT_DIRECTORY ${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY})
set (VERAPP_IMPORT_RULES ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES})

set (GENERATORS_COMMON_LIBRARY ${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY})

verapp_profile_check_source_files_conformance (${VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${GENERATORS_COMMON_LIBRARY}
                                               ${VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY})


set (CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/cpp_subprocess_string_replacement_generator.cpp)

add_executable (${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR}
                ${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR_SRCS})

target_link_libraries (${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR}
                       ${CPP_SUBPROCESS_GENERATORS_COMMON_LIBRARY})

set (REPLACEMENT_GENERATOR ${CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR})

verapp_profile_check_source_files_conformance (${VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${REPLACEMENT_GENERATOR}
                                               ${VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${REPLACEMENT_GENERATOR})
