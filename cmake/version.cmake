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


FUNCTION(omni_set_version MINOR MAJOR PATCH)
  set( OMNI_MAJOR_VERSION ${MINOR} CACHE STRING "Major Version for Omnidome")
  set( OMNI_MINOR_VERSION ${MAJOR} CACHE STRING "Minor Version for Omnidome")
  set( OMNI_PATCH_VERSION ${PATCH} CACHE STRING "Patch Version for Omnidome")
  set( OMNI_VERSION_STRING
      "${OMNI_MAJOR_VERSION}.${OMNI_MINOR_VERSION}.${OMNI_PATCH_VERSION}" CACHE STRING "Omnidome Version String")
  
  add_compile_definitions(OMNI_VERSION_STRING="${OMNI_VERSION_STRING}")
  add_compile_definitions(OMNI_MAJOR_VERSION=${OMNI_MAJOR_VERSION})
  add_compile_definitions(OMNI_MINOR_VERSION=${OMNI_MINOR_VERSION})
  add_compile_definitions(OMNI_PATCH_VERSION=${OMNI_PATCH_VERSION})
  MESSAGE(STATUS "Omnidome ${OMNI_VERSION_STRING}")
ENDFUNCTION()

find_package(Git)

if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_SOURCE_DIR} rev-list --max-count=1 HEAD
        OUTPUT_VARIABLE OMNI_GIT_REVISION
        ERROR_QUIET
        )
    if(NOT ${OMNI_GIT_REVISION} STREQUAL "")
        string(STRIP ${OMNI_GIT_REVISION} OMNI_GIT_REVISION)
    endif()
    message(STATUS "Current git revision is ${OMNI_GIT_REVISION}")
    set(OMNI_GIT_REVISION CACHE STRING "Git revision string")
    add_compile_definitions(OMNI_GIT_REVISION="${OMNI_GIT_REVISION}")

else()
    set(OMNI_GIT_REVISION "unknown")
endif()
