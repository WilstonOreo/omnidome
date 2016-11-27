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

# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND MATCHES YES)
  omni_option(DOXYGEN_FILE_ABS_PATH "Path of Doxygen file" ${CMAKE_PROJECT_DOC_DIR}/${PROJECT_NAME}.doxygen) 

  if (EXISTS ${DOXYGEN_FILE_ABS_PATH})
    CONFIGURE_FILE(${DOXYGEN_FILE_ABS_PATH} ${CMAKE_PROJECT_DOC_DIR} @ONLY)
    ADD_CUSTOM_TARGET(doc
      ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE_ABS_PATH}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen (see ./doc/html/index.html)" VERBATIM
    )
    MESSAGE(STATUS "Doxygen is setup. Use 'make doc' to generate documentation.")
  ELSE()
    MESSAGE(WARNING "Could not setup Doxygen since doxygen file ${DOXYGEN_FILE_ABS_PATH} does not exist!")

  endif()
ENDIF()
