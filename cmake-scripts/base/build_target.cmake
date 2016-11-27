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

# Macros for building executables or libraries in folders
# with a predefined and fixed directory structure.
# Build in Qt support is activated when QT_FOUND is defined.


MACRO(omni_postsetup_build_target BUILD_TARGET SOURCE_PATH )
  if(EXISTS ${SOURCE_PATH}/postbuild.cmake)
    include(${SOURCE_PATH}/postbuild.cmake)
  endif()
ENDMACRO()


# Scan for files for BUILD_TARGET in SOURCE_PATH
MACRO(omni_setup_build_target BUILD_TARGET SOURCE_PATH)
  SET(${BUILD_TARGET}_PATH ${SOURCE_PATH})

  if(EXISTS ${SOURCE_PATH}/build.cmake)
      include(${SOURCE_PATH}/build.cmake)
  endif()

  # Build target might be ignored if IGNORE flag is set
  if (${BUILD_TARGET}_IGNORE)
    MESSAGE(STATUS "${BUILD_TARGET} will be ignored.")
    return()
  endif()

  include_directories(${${BUILD_TARGET}_INCLUDE})

  if (${BUILD_TARGET}_INCLUDE)
    FILE(GLOB_RECURSE ${BUILD_TARGET}_HEADER_SOURCES
      ${${BUILD_TARGET}_INCLUDE}/*.h
      ${${BUILD_TARGET}_INCLUDE}/*.hpp
    )
  endif()
  
  if (NOT ${BUILD_TARGET}_CPP_SOURCES)
    FILE(GLOB_RECURSE ${BUILD_TARGET}_CPP_SOURCES
      ${SOURCE_PATH}/*.cpp
      ${SOURCE_PATH}/*.h
      ${SOURCE_PATH}/*.hpp
    )
  endif()

  SET(${BUILD_TARGET}_SOURCES 
    ${${BUILD_TARGET}_SOURCES} 
    ${${BUILD_TARGET}_CPP_SOURCES} )

  # On MacOSX, look for ObjectiveC and Objective-C++ sources
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
    # Look for ObjectiveC sources
    FILE(GLOB_RECURSE ${BUILD_TARGET}_OBJC_SOURCES ${SOURCE_PATH}/*.m )
    if (${BUILD_TARGET}_OBJC_SOURCES)
      set_source_files_properties(${BUILD_TARGET}_OBJC_SOURCES PROPERTIES
            COMPILE_FLAGS "-x objective-c")
      SET(${BUILD_TARGET}_SOURCES ${${BUILD_TARGET}_SOURCES} ${${BUILD_TARGET}_OBJC_SOURCES})
    endif()

    # Look for ObjectiveC++ sources
    FILE(GLOB_RECURSE ${BUILD_TARGET}_OBJCPP_SOURCES ${SOURCE_PATH}/*.mm )
    if (${BUILD_TARGET}_OBJCPP_SOURCES)
      set_source_files_properties(${BUILD_TARGET}_OBJCPP_SOURCES PROPERTIES
                                  COMPILE_FLAGS "-x objective-c++")
      SET(${BUILD_TARGET}_SOURCES ${${BUILD_TARGET}_SOURCES} ${${BUILD_TARGET}_OBJCPP_SOURCES})
    endif()
  ENDIF()

  SET(${BUILD_TARGET}_SOURCES
      ${${BUILD_TARGET}_SOURCES}
      ${${BUILD_TARGET}_HEADER_SOURCES}
  )

  # Setup Qt
  if (DEFINED QT_FOUND AND NOT ${BUILD_TARGET}_NO_QT)
    if (${BUILD_TARGET}_HEADERS_MOC_PATH)
      FILE(GLOB_RECURSE ${BUILD_TARGET}_HEADERS
        ${${BUILD_TARGET}_HEADERS_MOC_PATH}/*.h )
    endif()

    FILE(GLOB_RECURSE ${BUILD_TARGET}_UI_FORMS ${SOURCE_PATH}/ui/*.ui )
    FILE(GLOB_RECURSE ${BUILD_TARGET}_RESOURCES ${SOURCE_PATH}/*.qrc )

    QT5_WRAP_CPP(${BUILD_TARGET}_HEADERS_MOC ${${BUILD_TARGET}_HEADERS})
    QT5_ADD_RESOURCES(${BUILD_TARGET}_RESOURCES_RCC ${${BUILD_TARGET}_RESOURCES})
    QT5_WRAP_UI(${BUILD_TARGET}_UI_FORMS_HEADERS ${${BUILD_TARGET}_UI_FORMS})

    SET(${BUILD_TARGET}_SOURCES
      ${${BUILD_TARGET}_SOURCES}
      ${${BUILD_TARGET}_HEADERS_MOC}
      ${${BUILD_TARGET}_UI_FORMS_HEADERS}
      ${${BUILD_TARGET}_RESOURCES_RCC}
    )
  endif()

ENDMACRO()

# Make build target for shared library, is installed by default
MACRO(omni_add_shared_library BUILD_TARGET SOURCE_PATH)
  omni_setup_build_target(${BUILD_TARGET} ${SOURCE_PATH})

  ADD_LIBRARY(${BUILD_TARGET} SHARED ${${BUILD_TARGET}_SOURCES})
  target_compile_definitions(${BUILD_TARGET} PRIVATE ${${BUILD_TARGET}_DEFINITIONS})


  TARGET_LINK_LIBRARIES(${BUILD_TARGET} ${${BUILD_TARGET}_LIBRARIES})

  INSTALL(TARGETS ${BUILD_TARGET} DESTINATION ${OMNI_INSTALL_PATH}/lib )

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
    # LINK FRAMEWORKS
    if (${BUILD_TARGET}_FRAMEWORKS)
        target_link_libraries(${BUILD_TARGET} ${${BUILD_TARGET}_FRAMEWORKS} )
    endif()
  ENDIF()

  omni_postsetup_build_target(${BUILD_TARGET} ${SOURCE_PATH})
ENDMACRO()

# Make build target for executable, is NOT installed by default
MACRO(omni_add_executable BUILD_TARGET SOURCE_PATH)
  omni_setup_build_target(${BUILD_TARGET} ${SOURCE_PATH})

  # Main Application
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    IF(${BUILD_TARGET}_IS_MAIN_APP) # Make bundle only for main app
      ADD_EXECUTABLE(${BUILD_TARGET} MACOSX_BUNDLE ${${BUILD_TARGET}_SOURCES} )
    ELSE()
      ADD_EXECUTABLE(${BUILD_TARGET} ${${BUILD_TARGET}_SOURCES} )
    ENDIF()
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_EXECUTABLE(${BUILD_TARGET} ${${BUILD_TARGET}_SOURCES} )
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  TARGET_LINK_LIBRARIES(${BUILD_TARGET} ${${BUILD_TARGET}_LIBRARIES})

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
    # LINK FRAMEWORKS
    if (${BUILD_TARGET}_FRAMEWORKS)
        target_link_libraries(${BUILD_TARGET} ${${BUILD_TARGET}_FRAMEWORKS} )
    endif()
  ENDIF()

  target_compile_definitions(${BUILD_TARGET} PRIVATE ${${BUILD_TARGET}_DEFINITIONS})
  
  omni_postsetup_build_target(${BUILD_TARGET} ${SOURCE_PATH})
ENDMACRO()


MACRO(omni_mainapp BUILD_TARGET)

  SET(OMNI_MAINAPP ${BUILD_TARGET})
  SET(${BUILD_TARGET}_IS_MAIN_APP YES)

  # Set main include dir headers source as build target sources
  SET(${BUILD_TARGET}_SOURCES ${${CMAKE_PROJECT_NAME}_HEADER_SOURCES})

  include_directories(${OMNI_MAINAPP_SOURCE_PATH})
  omni_add_executable(${BUILD_TARGET} ${OMNI_MAINAPP_SOURCE_PATH})

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux Install properties
    INCLUDE(InstallRequiredSystemLibraries)
    INSTALL(TARGETS ${BUILD_TARGET} DESTINATION ${OMNI_INSTALL_PATH}/bin )

    # Install Start script
    INSTALL(FILES ${OMNI_DEPLOYMENT_RESOURCE_PATH}/${BUILD_TARGET}.sh
	     RENAME ${BUILD_TARGET}
          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_EXECUTE GROUP_READ
		                  WORLD_EXECUTE WORLD_READ
	     DESTINATION bin)

    # Install desktop file
    INSTALL(FILES ${OMNI_DEPLOYMENT_RESOURCE_PATH}/${BUILD_TARGET}.desktop
	     DESTINATION share/applications )

    # Install icon
    INSTALL(FILES ${OMNI_DEPLOYMENT_RESOURCE_PATH}/${BUILD_TARGET}.png
	     DESTINATION ${OMNI_INSTALL_PATH} )
  ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # MacOSX install properties
    SET(APP_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_TARGET}.app )
    SET(MACOSX_BUNDLE_INFO_STRING "${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_GUI_IDENTIFIER "com.${OMNI_VENDOR_SHORT}.${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${VERSION_STRING}")
    SET(MACOSX_BUNDLE_BUNDLE_NAME "${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_BUNDLE_PACKAGE_TYPE "APPL")
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_STRING}")
    SET(MACOSX_BUNDLE_ICON_FILE "${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_COPYRIGHT "(C) ${OMNI_YEAR}. ${OMNI_URL}")

    SET_SOURCE_FILES_PROPERTIES("${OMNI_DEPLOYMENT_RESOURCE_PATH}/${MACOSX_BUNDLE_ICON_FILE}.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
    )

    set_target_properties(${BUILD_TARGET} PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST "${OMNI_DEPLOYMENT_RESOURCE_PATH}/Info.plist")

    # Make directory for icon and copy it to Contents/Resources
    add_custom_command(TARGET ${BUILD_TARGET} PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory
                      ${APP_OUTPUT_DIR}/Contents/Resources
    )

    add_custom_command(TARGET ${BUILD_TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       "${OMNI_DEPLOYMENT_RESOURCE_PATH}/${BUILD_TARGET}.icns"
                       "${APP_OUTPUT_DIR}/Contents/Resources" )

  ENDIF()

  if (QT_FOUND AND NOT ${BUILD_TARGET}_NO_QT)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      # Deploy target for MacOSX deploy
      ADD_CUSTOM_TARGET(
            deploy
            ${QT5_LOCATION}/bin/macdeployqt ${APP_OUTPUT_DIR} -dmg
              DEPENDS ${BUILD_TARGET}
              WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
              COMMENT "Make DMG file for MacOS X."
              VERBATIM
      )
    ENDIF()

    deploy_qt()
  endif()
ENDMACRO()
