################################################################################
# This file is part of Omnidome.
#
# Copyright (c) 2016 by Michael Winkelmann.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

# Set default compiler flags for C and C++
MACRO(omni_common_cxx_flags)
  ADD_DEFINITIONS(-fPIC -fsigned-char -Wall -Wno-missing-braces)

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    ADD_DEFINITIONS("-g -DDEBUG -O1")
  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

  IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
    ADD_DEFINITIONS("-O3")

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Set clang as standard compiler
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Release")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_DEFINITIONS(-g -Os -fsigned-char -Wall -Wno-unknown-pragmas)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64 -Wno-unused-variable -Wno-unknown-pragmas ")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wno-unused-variable -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64 ")
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  IF (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_DEFINITIONS(-ferror-limit=5 -fcolor-diagnostics -fdiagnostics-show-template-tree  -Wno-deprecated )
  ENDIF()
ENDMACRO()


# Find compile with version number
MACRO(omni_find_compiler VAR EXECUTABLE VERSION)
  
    if ("${VERSION}" STREQUAL "") 
      SET (COMPILER_PATH ${EXECUTABLE})
    else()
      SET (COMPILER_PATH ${EXECUTABLE}-${VERSION})
    endif()
        
    if(NOT EXISTS "${${VAR}}")
      MESSAGE(WARNING "Compiler does not exist, using default.") 
    endif()

    set(${VAR} ${COMPILER_PATH})

ENDMACRO()


# Setup C compiler
MACRO(omni_setup_c_compiler EXECUTABLE VERSION) 
    omni_find_compiler(CMAKE_C_COMPILER ${EXECUTABLE} "${VERSION}")

    SET (CMAKE_C_FLAGS"-Wall -std=cc99")
    SET (CMAKE_C_FLAGS_DEBUG  "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE"-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
ENDMACRO()

# Setup used C++ compiler
MACRO(omni_setup_cxx_compiler EXECUTABLE VERSION)
  omni_find_compiler(CMAKE_CXX_COMPILER ${EXECUTABLE} "${VERSION}")
 
  SET (CMAKE_CXX_FLAGS"-Wall")
  SET (CMAKE_CXX_FLAGS_DEBUG  "-g")
  SET (CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
  SET (CMAKE_CXX_FLAGS_RELEASE"-O4 -DNDEBUG")
  SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
ENDMACRO()

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

if (NOT OMNI_CUSTOM_TOOLCHAIN) 
  # On Linux, use clang compiler (if present) for debugging and GNU for release
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      omni_setup_c_compiler("/usr/bin/clang" "${OMNI_CLANG_VERSION}")
      omni_setup_cxx_compiler("/usr/bin/clang++" "${OMNI_CLANG_VERSION}")
  elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
      omni_setup_c_compiler("/usr/bin/cc" "")
      omni_setup_cxx_compiler("/usr/bin/c++" "")
  endif()
ENDIF()

omni_common_cxx_flags()


MESSAGE(STATUS "C compiler is ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "C++ compiler is ${CMAKE_CXX_COMPILER}")

