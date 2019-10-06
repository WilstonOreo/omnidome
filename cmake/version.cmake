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


FUNCTION(omni_set_version_for_project MINOR MAJOR PATCH PROJECT_NAME) 
  string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

  set( ${PROJECT_NAME}_MAJOR_VERSION ${MINOR} CACHE STRING "Major Version for ${PROJECT_NAME}")
  set( ${PROJECT_NAME}_MINOR_VERSION ${MAJOR} CACHE STRING "Minor Version for ${PROJECT_NAME}")
  set( ${PROJECT_NAME}_PATCH_VERSION ${PATCH} CACHE STRING "Patch Version for ${PROJECT_NAME}")
  set( ${PROJECT_NAME}_VERSION_STRING 
      "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}" CACHE STRING "${PROJECT_NAME} Version String")
  
  add_definitions(
    -D${PROJECT_NAME_UPPER}_VERSION_STRING="${${PROJECT_NAME}_VERSION_STRING}"
    -D${PROJECT_NAME_UPPER}_MAJOR_VERSION=${${PROJECT_NAME}_MAJOR_VERSION}
    -D${PROJECT_NAME_UPPER}_MINOR_VERSION=${${PROJECT_NAME}_MINOR_VERSION}
    -D${PROJECT_NAME_UPPER}_PATCH_VERSION=${${PROJECT_NAME}_PATCH_VERSION}
  )
ENDFUNCTION()


# Set version of the main app
FUNCTION (omni_set_version MINOR MAJOR PATCH)
  omni_set_version_for_project(${MINOR} ${MAJOR} ${PATCH} Omnidome)
  MESSAGE(STATUS "${OMNI_PROJECT_NAME} ${${OMNI_PROJECT_NAME}_VERSION_STRING}")
ENDFUNCTION()
