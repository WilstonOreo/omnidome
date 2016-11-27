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

# Qt Setup
include(CMakeParseArguments)

# Qt major version, default is 5
omni_option(QT_MAJOR_VERSION "Qt Major Version" 5 )

# Qt Minor version, default is 6
omni_option(QT_MINOR_VERSION "Qt Minor Version" 6 )
omni_option(QT_PATH "Qt Path" "$ENV{HOME}/Qt" )

MACRO(find_qt5_component COMPONENT_NAME)

  set(Qt5${COMPONENT_NAME}_DIR ${QT_LIBRARY_DIR}/cmake/Qt5${COMPONENT_NAME} )
  find_package(Qt5${COMPONENT_NAME} REQUIRED)

  if("${Qt5_LIBRARIES}" STREQUAL "")
    set(Qt5_LIBRARIES Qt5::${COMPONENT_NAME})
  else()
    set(Qt5_LIBRARIES ${Qt5_LIBRARIES} Qt5::${COMPONENT_NAME})
  endif()

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(${Qt5${COMPONENT_NAME}_INCLUDE_DIRS})
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include_directories(${QT_INCLUDE_DIR}/Qt${COMPONENT_NAME})

    # Set module as install target
    #   INSTALL(
    #   FILES
    #    "${QT_LIBRARY_DIR}/libQt5${COMPONENT_NAME}.so.${QT_VERSION}.0"
    #    RENAME "libQt5${COMPONENT_NAME}.so.${QT_MAJOR_VERSION}"
    #    DESTINATION ${OMNI_INSTALL_PATH}/lib
    #)
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  if (${COMPONENT_NAME} MATCHES "Widgets")
    set(Qt5${COMPONENT_NAME}_UIC_EXECUTABLE ${Qt5_LOCATION}/bin/uic)
    
    # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
    add_definitions(${Qt5Widgets_DEFINITIONS})
  endif (${COMPONENT_NAME} MATCHES "Widgets")

ENDMACRO(find_qt5_component COMPONENT_NAME)

MACRO (deploy_qt)
  return()
   IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    # X Server support
    INSTALL(FILES ${QT_LIBRARY_DIR}/../plugins/platforms/libqxcb.so
	      DESTINATION ${OMNI_INSTALL_PATH}/plugins/platforms )
    INSTALL(FILES ${QT_LIBRARY_DIR}/../plugins/xcbglintegrations/libqxcb-egl-integration.so
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ
		    WORLD_EXECUTE WORLD_READ
  	DESTINATION ${OMNI_INSTALL_PATH}/plugins/xcbglintegrations )

    INSTALL(FILES ${QT_LIBRARY_DIR}/../plugins/xcbglintegrations/libqxcb-glx-integration.so

        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ
		    WORLD_EXECUTE WORLD_READ
	DESTINATION ${OMNI_INSTALL_PATH}/plugins/xcbglintegrations
	)

        INSTALL(FILES
            "${QT_LIBRARY_DIR}/libQt5XcbQpa.so.${QT_VERSION}.0"
            RENAME "libQt5XcbQpa.so.${QT_MAJOR_VERSION}"
            DESTINATION ${OMNI_INSTALL_PATH}/lib
        )

        INSTALL(FILES
            "${QT_LIBRARY_DIR}/libQt5DBus.so.${QT_VERSION}.0"
            RENAME "libQt5DBus.so.${QT_MAJOR_VERSION}"
            DESTINATION ${OMNI_INSTALL_PATH}/lib
        )
  ENDIF()
ENDMACRO (deploy_qt)

# Qt5 Setup
MACRO(setup_qt)
    SET(_moc ${CMAKE_SOURCE_DIR}/moc )

  SET(QT_FOUND FALSE)
  SET(QT_VERSION "${QT_MAJOR_VERSION}.${QT_MINOR_VERSION}")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(Qt5_LOCATION "${QT_PATH}/${QT_VERSION}/clang_64")
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(Qt5_LOCATION "${QT_PATH}/${QT_VERSION}/gcc_64")
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  
  if(NOT IS_DIRECTORY ${Qt5_LOCATION})
    return()
  endif()

  set(QT_QMAKE_EXECUTABLE "${Qt5_LOCATION}/bin/qmake" )
  set(QT_MOC_EXECUTABLE "${Qt5_LOCATION}/bin/moc"  )
  set(QT_RCC_EXECUTABLE "${Qt5_LOCATION}/bin/rcc" )
  set(QT_UIC_EXECUTABLE "${Qt5_LOCATION}/bin/uic" )
  set(QT_INCLUDE_DIR "${Qt5_LOCATION}/include" )
  set(QT_LIBRARY_DIR "${Qt5_LOCATION}/lib" )
  
  MESSAGE(STATUS "Using Qt ${QT_VERSION}")



  FOREACH(LIB icui18n icuuc icudata ) 
    if (EXISTS "${QT_LIBRARY_DIR}/lib${LIB}.so.${QT_MAJOR_VERSION}${QT_MINOR_VERSION}")
      MESSAGE("${LIB} found.")
    endif()
    #    INSTALL(FILES
    #      "${QT_LIBRARY_DIR}/lib${LIB}.so.${QT_MAJOR_VERSION}${QT_MINOR_VERSION}"
    #        RENAME "lib${LIB}.so.${QT_MAJOR_VERSION}${QT_MINOR_VERSION}"
    #        DESTINATION ${OMNI_INSTALL_PATH}/lib
    #  )
  ENDFOREACH(LIB)

  if (NOT DEFINED QT_MODULES)
    set(QT_MODULES "")
    LIST(APPEND QT_MODULES "Core")
    LIST(APPEND QT_MODULES "Gui")
    LIST(APPEND QT_MODULES "Widgets")
    LIST(APPEND QT_MODULES "OpenGL")
  endif()

  foreach(QT_MODULE ${QT_MODULES})
    message(STATUS "Using Qt Module: ${QT_MODULE}")
    find_qt5_component(${QT_MODULE})
  endforeach()

  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
  include_directories(${QT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/moc)


  
  IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    ADD_DEFINITIONS("-DQT_DEBUG")
  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

  IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
    ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Release")
  
  set(QT_FOUND TRUE)
 
ENDMACRO()


################################
setup_qt()

if(QT_FOUND EQUAL FALSE)
  MESSAGE(FATAL_ERROR "\
  Qt Framework was not found in ${QT_PATH}. You might set the absolute path QT_PATH manually: \n \
  \tcmake . -DQT_PATH=/absolute/path/to/qt \n \
  Qt is looked up by default in $ENV{HOME}/Qt . \
  ") 
endif()
