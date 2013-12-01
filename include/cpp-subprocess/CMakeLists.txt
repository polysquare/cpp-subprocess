# /include/cpp-subprocess/CMakeLists.txt
# Ensure that the cpp-subprocess headers are scanned for source code style compliance
#
# See LICENCE.md for Copyright information

set (CPP_SUBPROCESS_HEADERS
     ${CMAKE_CURRENT_SOURCE_DIR}/fdbackup.h
     ${CMAKE_CURRENT_SOURCE_DIR}/launch.h
     ${CMAKE_CURRENT_SOURCE_DIR}/locatebinary.h
     ${CMAKE_CURRENT_SOURCE_DIR}/operating_system.h
     ${CMAKE_CURRENT_SOURCE_DIR}/pipe.h
     ${CMAKE_CURRENT_SOURCE_DIR}/readfd.h
     ${CMAKE_CURRENT_SOURCE_DIR}/redirectedfd.h)
set (CPP_SUBPROCESS_CHECK_HEADERS_COMPLIANCE_TARGET cpp_subprocess_check_headers_compliance)

# Create a new "proxy" target and set the SOURCES property on it
# so that we can scan the source files for compliance when it is built
add_custom_target (${CPP_SUBPROCESS_CHECK_HEADERS_COMPLIANCE_TARGET} ALL
                   COMMENT "Checking headers compliance")

# Scan that proxy target for compliance
verapp_profile_check_source_files_conformance_for_target (${CPP_SUBPROCESS_VERAPP_OUTPUT_DIRECTORY}
                                                          CPP_SUBPROCESS_HEADERS
                                                          ${CPP_SUBPROCESS_VERAPP_PROFILE}
                                                          ${CPP_SUBPROCESS_CHECK_HEADERS_COMPLIANCE_TARGET}
                                                          ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES}
                                                          ERROR)
