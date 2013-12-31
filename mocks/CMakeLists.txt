# /mocks/CMakeLists.txt
# Build polysquare_cpp_subprocess_mocks library.
#
# See LICENCE.md for Copyright information.

include_directories (${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
                     ${CPP_SUBPROCESS_TEST_INCLUDE_DIRS}
	                   ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}
	                   ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY})

set (CPP_SUBPROCESS_MOCKS_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/mock_operating_system.cpp)

add_library (${CPP_SUBPROCESS_MOCKS} STATIC
             ${CPP_SUBPROCESS_MOCKS_SRCS})

target_link_libraries (${CPP_SUBPROCESS_MOCKS}
                       ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES}
                       ${CPP_SUBPROCESS_TEST_LIBRARIES})

verapp_profile_check_source_files_conformance (${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${CPP_SUBPROCESS_MOCKS}
                                               ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${CPP_SUBPROCESS_MOCKS}
                         INCLUDES
                         ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIR}
                         ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIR})