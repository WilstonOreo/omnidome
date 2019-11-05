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

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(OMNI_PLUGIN_PATH ${CMAKE_SOURCE_DIR}/plugins )

function(omni_plugin BUILD_TARGET SOURCES)
    if(APPLE)
      set(OUTPUT_PATH ../../../app/omnidome.app/Contents/PlugIns)
    elseif(WIN32)
      set(OUTPUT_PATH ../../../app/plugins)
    elseif(UNIX)
      set(OUTPUT_PATH ../../../app/plugins)
    endif()
    add_library(${BUILD_TARGET} SHARED ${SOURCES})
    target_include_directories(${BUILD_TARGET}
        PUBLIC
            ../../../lib/include
            ../../../widgetlib/include
        PRIVATE
            .
    )

    SET_TARGET_PROPERTIES(${BUILD_TARGET}
        PROPERTIES
        SUFFIX ".omnix"
        PREFIX ""
    )

    SET_TARGET_PROPERTIES(${BUILD_TARGET}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}
        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}
    )

    # Make directory for icon and copy it to Contents/Resources
 #   add_custom_command(TARGET ${BUILD_TARGET} PRE_BUILD
 #                     COMMAND ${CMAKE_COMMAND} -E make_directory
 #                     ${OUTPUT_PATH})
    target_link_libraries(${BUILD_TARGET}
        PUBLIC
            omni
            omniwidget
    )

    target_compile_definitions(${BUILD_TARGET}
        PUBLIC
        OMNIPLUGIN_LIBRARY)


    # Make install target for linux in /usr/share/${PROJECT_NAME}/plugins
# IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#      INSTALL(TARGETS ${BUILD_TARGET} DESTINATION ${OMNI_INSTALL_PATH}}/plugins)
#    ENDIF()
endfunction()
