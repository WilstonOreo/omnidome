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

# Adds a git submodule which also uses cm8kr as build system
# It loads a build.cmake with exported files if present
MACRO(omni_add_submodule module_path)
  # Disable INTERFACE_LINK_LIBRARIES warning
  cmake_policy(SET CMP0022 OLD)

  SET(abs_module_path ${CMAKE_SOURCE_DIR}/${module_path} )

  # Add submodule only if we are in the project root folder
  if (${CMAKE_SOURCE_DIR} MATCHES ${PROJECT_SOURCE_DIR})
    MESSAGE("Adding submodule ${module_path}")

    INCLUDE_DIRECTORIES(
      ${abs_module_path}/include
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${module_path}
    )
    # Include build.cmake file
    if(EXISTS ${abs_module_path}/build.cmake)
      include(${abs_module_path}/build.cmake)
    endif()
  endif()

ENDMACRO()
