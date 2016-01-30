# Get CMake Macro path from current file (nice hack)

if ("${CMAKE_MACROS_DIR}" STREQUAL "")

  get_filename_component(CMAKE_MACROS_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

  MESSAGE(STATUS "Found CMakeMacros environment in ${CMAKE_MACROS_DIR}")

  include(${CMAKE_MACROS_DIR}/build_directories.cmake)

  # MacOSX specific setup
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

      # use, i.e. don't skip the full RPATH for the build tree
      SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

      # when building, don't use the install RPATH already
      # (but later on when installing)
      SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

      SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

      # add the automatically determined parts of the RPATH
      # which point to directories outside the build tree to the install RPATH
      SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

      # the RPATH to be used when installing, but only if it's not a system directory
      LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
      IF("${isSystemDir}" STREQUAL "-1")
          SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
      ENDIF("${isSystemDir}" STREQUAL "-1")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  # Import macro for convenient loading of macros
  # Usage: import(macro1 macro2)
  # File extension "cmake" is appended automaticcly
  FUNCTION (import ${ARGN})
    # Cannot use ARGN directly with list() command.
    # Copy to a variable first.
    set (macro_args ${ARGN})

    foreach(module_name ${macro_args})
      if ("${${module_name}_LOADED}" STREQUAL "")
        set(module_filename ${CMAKE_MACROS_DIR}/${module_name}.cmake)
        set(${module_name}_LOADED YES PARENT_SCOPE)
        INCLUDE(${module_filename})
      endif()
    endforeach(module_name)
  ENDFUNCTION(import)

endif()

include_directories(BEFORE
  ${PROJECT_SOURCE_DIR}/include
)
