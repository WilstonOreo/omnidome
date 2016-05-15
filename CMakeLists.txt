# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(Omnidome)

include(cmake-files/base.cmake)

import(
  compiler_flags
  doxygen
  qt
  gl
  cpack
  plugin
)

set( CMAKE_MACOSX_RPATH 1)
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

set( MAJOR_VERSION 1 )
set( MINOR_VERSION 0 )
set( PATCH_VERSION 0 )

################################
if(DEFINED QT_PATH)
    MESSAGE(STATUS "Qt Path: ${QT_PATH}")
    setup_qt(5.6 ${QT_PATH})
else()
    detect_qt()
endif()

if(NOT QT_FOUND)
    MESSAGE(FATAL_ERROR "Qt Framework was not found. You might set QT_PATH manually.")
    return()
endif()

add_definitions(-DOMNIDOME_VERSION_STRING="1.0.0.0")

########### BUILD_TARGETS ############


common_cxx_flags()

################################
# Setup libomni build target

FILE(GLOB_RECURSE omni_lib_cpp_files ${CMAKE_SOURCE_DIR}/src/lib/*.cpp )
FILE(GLOB_RECURSE omni_lib_ui_headers
  ${CMAKE_SOURCE_DIR}/include/omni/ui/*.h )

FILE(GLOB_RECURSE omni_lib_ui_forms ${CMAKE_SOURCE_DIR}/src/lib/ui/*.ui )
FILE(GLOB_RECURSE omni_lib_resources ${CMAKE_SOURCE_DIR}/src/lib/*.qrc )

QT5_WRAP_CPP(omni_lib_ui_headers_moc ${omni_lib_ui_headers})
QT5_ADD_RESOURCES(omni_lib_resources_rcc ${omni_lib_resources})
QT5_GENERATE_UI(omni_lib_forms_headers ${omni_lib_ui_forms})

SET(omni_lib_src
    ${omni_lib_cpp_files}
    ${omni_lib_ui_headers_moc}
    ${omni_lib_ui_forms_headers}
    ${omni_lib_resources_rcc}
)

ADD_LIBRARY(omni SHARED ${omni_lib_src})
TARGET_LINK_LIBRARIES(omni ${Qt5_LIBRARIES} ${omni_gl_LIBRARIES} )

# END Setup libomni build target
################################

########### Plugins ############

# Load canvas plugins
load_plugins(${CMAKE_SOURCE_DIR}/src/modules/canvas)

# Load input plugins
load_plugins(${CMAKE_SOURCE_DIR}/src/modules/input)

# Load mapping inputs
load_plugins(${CMAKE_SOURCE_DIR}/src/modules/mapping)

################################
# Setup omnidome executable

include_directories(${CMAKE_SOURCE_DIR}/src/app)
FILE(GLOB_RECURSE omni_app_cpp_files
  ${CMAKE_SOURCE_DIR}/src/app/*.cpp
  )

FILE(GLOB_RECURSE omni_headers ${CMAKE_SOURCE_DIR}/src/app/*.h )
FILE(GLOB_RECURSE omni_forms ${CMAKE_SOURCE_DIR}/src/app/ui/*.ui )
FILE(GLOB_RECURSE omni_resources ${CMAKE_SOURCE_DIR}/src/app/rc/*.qrc )

QT5_WRAP_CPP(omni_headers_moc ${omni_headers})
QT5_ADD_RESOURCES(omni_resources_rcc ${omni_resources})
QT5_GENERATE_UI(omni_forms_headers ${omni_forms})
SET(omni_src ${omni_app_cpp_files} ${omni_headers_moc} ${omni_forms_headers} ${omni_resources_rcc})


# END Setup omnidome executable
################################


########### Tests #############
if(DEFINED OMNI_GSTREAMER)
  omni_gstreamer_test()
endif()

ADD_EXECUTABLE(test_Serialization
  ./src/test/Serialization.cpp )
TARGET_LINK_LIBRARIES(test_Serialization ${Qt5_LIBRARIES} ${omni_gl_LIBRARIES}  omni )

ADD_EXECUTABLE(test_ThrowRatio
  ./src/test/ThrowRatio.cpp )
TARGET_LINK_LIBRARIES(test_ThrowRatio ${Qt5_LIBRARIES} ${omni_gl_LIBRARIES}  omni )

# Main Application
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(OMNIDOME_APP_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/omnidome.app )

  SET(MACOSX_BUNDLE_INFO_STRING "Omnidome")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "com.cr8tr.omnidome")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version 1.0.0")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "Omnidome")
  SET(MACOSX_BUNDLE_BUNDLE_PACKAGE_TYPE "APPL")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0")
  SET(MACOSX_BUNDLE_ICON_FILE "omnidome_icon")

  SET(MACOSX_BUNDLE_COPYRIGHT "(C) 2016, Michael Winkelmann, cr8tr.org, omnido.me")

  SET_SOURCE_FILES_PROPERTIES("cmake-files/${MACOSX_BUNDLE_ICON_FILE}.icns"
    PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
  )

  ADD_EXECUTABLE(omnidome MACOSX_BUNDLE ${omni_src} )

  set_target_properties(omnidome PROPERTIES
               MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake-files/Info.plist")
  TARGET_LINK_LIBRARIES(omnidome omni ${omni_gl_LIBRARIES} ${Qt5_LIBRARIES} )



  # Make directory for icon and copy it to Contents/Resources
  add_custom_command(TARGET omnidome PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory
                  ${OMNIDOME_APP_OUTPUT_DIR}/Contents/Resources
                 )
  add_custom_command(TARGET omnidome POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   "${CMAKE_SOURCE_DIR}/cmake-files/omnidome_icon.icns"
                 "${OMNIDOME_APP_OUTPUT_DIR}/Contents/Resources" )

    # ${Qt5Core_INCLUDE_DIRS} is a list; I'm expecting the first entry to always be QtCore.framework.
    ADD_CUSTOM_TARGET(
        deploy_osx
        ${QT_PATH}/${QT_VERSION}/clang_64/bin/macdeployqt ${OMNIDOME_APP_OUTPUT_DIR} -dmg
        DEPENDS omnidome
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Make DMG file for MacOS X."
        VERBATIM
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  ADD_EXECUTABLE(omnidome ${omni_src} )
  TARGET_LINK_LIBRARIES(omnidome omni ${omni_extra_libs} ${Qt5_LIBRARIES} )

  INSTALL(TARGETS omnidome DESTINATION share/omnidome)
  INSTALL(TARGETS omni DESTINATION share/omnidome/libs)

  INSTALL(FILES cmake-files/omnidome.sh
	RENAME omnidome
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ
		    WORLD_EXECUTE WORLD_READ
	DESTINATION bin)

  INSTALL(FILES cmake-files/omnidome.desktop
	DESTINATION share/applications )
  INSTALL(FILES cmake-files/icon.png
	DESTINATION share/omnidome/ )

  INCLUDE(InstallRequiredSystemLibraries)
    FOREACH(LIB Qt5Core Qt5Gui Qt5Widgets Qt5OpenGL)
        INSTALL(FILES
            "${QT_LIBRARY_DIR}/lib${LIB}.so.${QT_VERSION}.0"
            RENAME "lib${LIB}.so.5"
            DESTINATION share/omnidome/libs
        )
    ENDFOREACH(LIB)

    FOREACH(LIB icui18n icuuc icudata )
        INSTALL(FILES
            "${QT_LIBRARY_DIR}/lib${LIB}.so.56.1"
            RENAME "lib${LIB}.so.56"
            DESTINATION share/omnidome/libs
        )
    ENDFOREACH(LIB)

    INSTALL(FILES ${QT_LIBRARY_DIR}/../plugins/platforms/libqxcb.so
	DESTINATION share/omnidome/plugins/platforms )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

setup_cpack()
