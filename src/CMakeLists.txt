# /src/CMakeLists.txt
# Build polysquare_cpp_subprocess library.
#
# See LICENCE.md for Copyright information.

include_directories (${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
	                   ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}
	                   ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY})

set (CPP_SUBPROCESS_LIBRARY_SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/fdbackup.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/launch.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/locatebinary.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/pipe.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/readfd.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/redirectedfd.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    set (CPP_SUBPROCESS_LIBRARY_SRCS
         ${CPP_SUBPROCESS_LIBRARY_SRCS}
         ${CMAKE_CURRENT_SOURCE_DIR}/darwin_operating_system.cpp)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    set (CPP_SUBPROCESS_LIBRARY_SRCS
         ${CPP_SUBPROCESS_LIBRARY_SRCS}
         ${CMAKE_CURRENT_SOURCE_DIR}/gnu_operating_system.cpp)

endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_library (${CPP_SUBPROCESS_LIBRARY} STATIC
             ${CPP_SUBPROCESS_LIBRARY_SRCS})

target_link_libraries (${CPP_SUBPROCESS_LIBRARY}
                       ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES})

verapp_profile_check_source_files_conformance (${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY}
                                               ${CMAKE_CURRENT_SOURCE_DIR}
                                               ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                               ${CPP_SUBPROCESS_LIBRARY}
                                               ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES}
                                               ERROR)

cppcheck_target_sources (${CPP_SUBPROCESS_LIBRARY}
                         INCLUDES
                         ${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIR}
                         ${CPP_SUBPROCESS_INTERNAL_SOURCE_DIR})