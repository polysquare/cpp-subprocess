# /CMakeLists.txt
# Main CMake file for cpp-subprocess.
#
# See LICENCE.md for Copyright Information.

project (CPPSubprocess)

cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

include ("${CMAKE_CURRENT_LIST_DIR}/conanbuildinfo.cmake")
conan_basic_setup ()

option (CPP_SUBPROCESS_BOOST_USE_STATIC_LIBS ON
        "Link static libraries for boost. OFF if not using system compiler")
set (Boost_USE_STATIC_LIBS ${CPP_SUBPROCESS_BOOST_USE_STATIC_LIBS})

include ("cmake/common-universal-cmake/PolysquareCommon")

psq_compiler_bootstrap ()
psq_rules_bootstrap ()
psq_gmock_bootstrap ()
psq_coverage_bootstrap ()
psq_acceleration_bootstrap ()
psq_sanitizers_bootstrap ()

function (cpp_subprocess_import_external_libraries_from_boost RETURN_VALUE
                                                              VERSION)

    cmake_parse_arguments (EXTERNAL
                           ""
                           ""
                           "COMPONENTS"
                           ${ARGN})

    find_package (Boost ${VERSION} COMPONENTS ${EXTERNAL_COMPONENTS})

    set (RETURN_LIST)

    foreach (LIBRARY ${Boost_LIBRARIES})

        list (FIND Boost_LIBRARIES ${LIBRARY} INDEX)
        list (GET EXTERNAL_COMPONENTS ${INDEX} EXT_COMPONENT_NAME)
        set (IMPORTED_LIBRARY_NAME imported_boost_${EXT_COMPONENT_NAME})

        add_library (${IMPORTED_LIBRARY_NAME} STATIC IMPORTED GLOBAL)
        set_target_properties (${IMPORTED_LIBRARY_NAME}
                               PROPERTIES
                               IMPORTED_LOCATION
                               ${LIBRARY})
        list (APPEND RETURN_LIST ${IMPORTED_LIBRARY_NAME})

    endforeach ()

    set (${RETURN_VALUE} ${RETURN_LIST} PARENT_SCOPE)

endfunction ()

cpp_subprocess_import_external_libraries_from_boost (BOOST_LIBRARIES
                                                     1.46
                                                     COMPONENTS
                                                     program_options
                                                     iostreams)

set (CPP_SUBPROCESS_LIBRARY psq_cpp_subprocess)
set (CPP_SUBPROCESS_MOCKS psq_cpp_subprocess_mocks)
set (CPP_SUBPROCESS_STRING_REPLACEMENT_GENERATOR
     cpp_subprocess_string_replacement_generator)

set (CPP_SUBPROCESS_EXTERNAL_LIBRARIES
     ${BOOST_LIBRARIES})

set (CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (CPP_SUBPROCESS_MOCKS_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
set (CPP_SUBPROCESS_GENERATORS_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}/generators)
set (CPP_SUBPROCESS_TESTS_DIRECTORY
     ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_subdirectory (${CPP_SUBPROCESS_INTERNAL_SOURCE_DIRECTORY})
add_subdirectory (${CPP_SUBPROCESS_MOCKS_DIRECTORY})
add_subdirectory (${CPP_SUBPROCESS_TESTS_DIRECTORY})
add_subdirectory (${CPP_SUBPROCESS_GENERATORS_DIRECTORY})
add_subdirectory (${CPP_SUBPROCESS_INTERNAL_INCLUDE_DIRECTORY})
