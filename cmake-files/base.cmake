# Get CMake Macro path from current file (nice hack)

if ("${CMAKE_MACROS_DIR}" STREQUAL "")

  get_filename_component(CMAKE_MACROS_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

  MESSAGE(STATUS "Found CMakeMacros environment in ${CMAKE_MACROS_DIR}")

  include(${CMAKE_MACROS_DIR}/build_directories.cmake)

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
