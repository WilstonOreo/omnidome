IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
SET(CMAKE_BUILD_TYPE "Debug")
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
MESSAGE(STATUS "Build mode is ${CMAKE_BUILD_TYPE}")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
  
  IF("${CLANG_VERSION}" STREQUAL "")
    SET(CLANG_VERSION "3.6")
  ENDIF()
  
  if (${CMAKE_BUILD_TYPE} MATCHES "Debug")   
    SET (CMAKE_C_COMPILER "/usr/bin/clang-${CLANG_VERSION}")
    SET (CMAKE_C_FLAGS"-Wall -std=c99")
    SET (CMAKE_C_FLAGS_DEBUG  "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE"-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

    SET (CMAKE_CXX_COMPILER "/usr/bin/clang++-${CLANG_VERSION}")
    SET (CMAKE_CXX_FLAGS"-Wall")
    SET (CMAKE_CXX_FLAGS_DEBUG  "-g")
    SET (CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELEASE"-O4 -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    SET (CMAKE_AR  "/usr/bin/llvm-ar")
    SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
    SET (CMAKE_NM  "/usr/bin/llvm-nm")
    SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
    SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
  endif()
  
  if (${CMAKE_BUILD_TYPE} MATCHES "Release")   
    SET (CMAKE_C_COMPILER "/usr/bin/cc")
    SET (CMAKE_C_FLAGS"-Wall -std=cc99")
    SET (CMAKE_C_FLAGS_DEBUG  "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE"-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

    SET (CMAKE_CXX_COMPILER "/usr/bin/c++")
    SET (CMAKE_CXX_FLAGS"-Wall")
    SET (CMAKE_CXX_FLAGS_DEBUG  "-g")
    SET (CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELEASE"-O4 -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    SET (CMAKE_AR  "/usr/bin/llvm-ar")
    SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
    SET (CMAKE_NM  "/usr/bin/llvm-nm")
    SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
    SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
  endif()
endif()


MESSAGE(STATUS "C++ compiler is ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C compiler is ${CMAKE_C_COMPILER}")


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
ENDIF()


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

include_directories(BEFORE
  ${PROJECT_SOURCE_DIR}/include
)
