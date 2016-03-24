# /src/CMakeLists.txt
#
# Build polysquare_cpp_subprocess library.
#
# See /LICENCE.md for Copyright information

set (CPP_SUBPROCESS_LIBRARY_SRCS
     "${CMAKE_CURRENT_SOURCE_DIR}/fdbackup.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/launch.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/locatebinary.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/pipe.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/readfd.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/redirectedfd.cpp")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    set (CPP_SUBPROCESS_LIBRARY_SRCS
         ${CPP_SUBPROCESS_LIBRARY_SRCS}
         "${CMAKE_CURRENT_SOURCE_DIR}/darwin_operating_system.cpp")

elseif ()

    set (CPP_SUBPROCESS_LIBRARY_SRCS
         ${CPP_SUBPROCESS_LIBRARY_SRCS}
         "${CMAKE_CURRENT_SOURCE_DIR}/gnu_operating_system.cpp")

endif ()

psq_add_library (${CPP_SUBPROCESS_LIBRARY} STATIC
                 SOURCES ${CPP_SUBPROCESS_LIBRARY_SRCS}
                 EXTERNAL_INCLUDE_DIRS
                 ${CPP_SUBPROCESS_EXTERNAL_INCLUDE_DIRS}
                 INTERNAL_INCLUDE_DIRS
                 "${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY}"
                 "${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY}"
                 LIBRARIES ${CPP_SUBPROCESS_EXTERNAL_LIBRARIES})
