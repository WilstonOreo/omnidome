# Adds a git submodule which also uses CMake as build system

MACRO(add_submodule module_path)
  # Disable INTERFACE_LINK_LIBRARIES warning
  cmake_policy(SET CMP0022 OLD) 

  SET(abs_module_path ${CMAKE_SOURCE_DIR}/../${module_path} )

  # Add submodule only if we are in the project root folder
  if (${CMAKE_SOURCE_DIR} MATCHES ${PROJECT_SOURCE_DIR}) 
    MESSAGE(STATUS "--- Adding submodule ${module_path}")

    INCLUDE_DIRECTORIES(
      ${abs_module_path}/include
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${module_path}
    )
    add_subdirectory(${abs_module_path} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${module_path}) 
    # Add include directories for submodule  
  endif()

ENDMACRO()
