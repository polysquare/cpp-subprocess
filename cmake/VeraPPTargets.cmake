# VeraPPTargets.cmake
# Set up some of the main targets that we will use in order
# to determine when the Vera++ rules need to be imported
#
# See LICENCE.md for Copyright information

include (VeraPPUtilities)

add_custom_target (${CPP_SUBPROCESS_VERAPP_IMPORT_RULES} ALL)

set (_import_target ${CPP_SUBPROCESS_VERAPP_IMPORT_RULES})
set (_rules_out_dir ${CPP_SUBPROCESS_VERAPP_RULES_OUTPUT_DIRECTORY})
set (_profiles_out_dir ${CPP_SUBPROCESS_VERAPP_PROFILES_OUTPUT_DIRECTORY})
set (_rules_in_dir ${CPP_SUBPROCESS_VERAPP_RULES_SOURCE_DIRECTORY})
set (_profiles_in_dir ${CPP_SUBPROCESS_VERAPP_PROFILES_SOURCE_DIRECTORY})

verapp_import_default_rules_into_subdirectory_on_target (${_rules_out_dir}
                                                         ${_import_target})

verapp_copy_files_in_dir_to_subdir_on_target (${_rules_in_dir}
                                              ${_rules_out_dir}
                                              .tcl
                                              cpp_subprocess_verapp_copy_rules_files
                                              "CPP-Subprocess Vera++ rule")

add_dependencies (${_import_target} cpp_subprocess_verapp_copy_rules_files)

verapp_copy_files_in_dir_to_subdir_on_target (${_profiles_in_dir}
                                              ${_profiles_out_dir}
                                              NO_MATCH
                                              cpp_subprocess_verapp_copy_profile_files
                                              "CPP-Subprocess Vera++ profile")

add_dependencies (${_import_target} cpp_subprocess_verapp_copy_profile_files)
