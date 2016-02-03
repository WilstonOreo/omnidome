# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(Omnidome)

include(cmake-files/base.cmake)

import(
  compiler_flags
  doxygen
  qt
  cpack
  plugin
)

set( CMAKE_MACOSX_RPATH 1)
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

################################
if(DEFINED QT_PATH)
    MESSAGE(STATUS "Qt Path: ${QT_PATH}")
    setup_qt(5.5 ${QT_PATH})
else()
    detect_qt()
endif()

if(NOT QT_FOUND)
    MESSAGE(FATAL_ERROR "Qt Framework was not found. You might set QT_PATH manually.")
    return()
endif()

add_definitions(-DOMNIDOME_VERSION_STRING="1.0")

########### BUILD_TARGETS ############

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(omni_extra_libs OpenGL )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(omni_extra_libs GL )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


common_cxx_flags()

################################
# Setup libomni build target

FILE(GLOB_RECURSE omni_lib_cpp_files ${CMAKE_SOURCE_DIR}/src/lib/*.cpp )
FILE(GLOB_RECURSE omni_lib_ui_headers ${CMAKE_SOURCE_DIR}/include/omni/ui/*.h ${CMAKE_SOURCE_DIR}/include/slim/*.h )
FILE(GLOB_RECURSE omni_lib_ui_forms ${CMAKE_SOURCE_DIR}/src/lib/ui/*.ui )
FILE(GLOB_RECURSE omni_lib_resources ${CMAKE_SOURCE_DIR}/src/lib/*.qrc )

QT5_WRAP_CPP(omni_lib_ui_headers_moc ${omni_lib_ui_headers})
QT5_ADD_RESOURCES(omni_lib_resources_rcc ${omni_lib_resources})
#QT5_GENERATE_UI(omni_lib_forms_headers ${omni_lib_ui_forms})

SET(omni_lib_src
    ${omni_lib_cpp_files}
    ${omni_lib_ui_headers_moc}
    ${omni_lib_ui_forms_headers}
    ${omni_lib_resources_rcc}
)

ADD_LIBRARY(omni SHARED ${omni_lib_src})
TARGET_LINK_LIBRARIES(omni ${Qt5_LIBRARIES} ${omni_extra_libs} )

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
TARGET_LINK_LIBRARIES(test_Serialization ${Qt5_LIBRARIES} ${omni_extra_libs}  omni )

# Main Application
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  SET(MACOSX_BUNDLE_INFO_STRING "Omnidome")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "com.cr8tr.omnidome")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version 1.0.0")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "Omnidome")
  SET(MACOSX_BUNDLE_BUNDLE_PACKAGE_TYPE "APPL")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0")
  SET(MACOSX_BUNDLE_ICON_FILE "omnidome_icon")

  SET(MACOSX_BUNDLE_COPYRIGHT "(C) 2015, Michael Winkelmann, cr8tr.org, omnido.me")

  SET_SOURCE_FILES_PROPERTIES("cmake-files/${MACOSX_BUNDLE_ICON_FILE}.icns"
    PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
  )

  ADD_EXECUTABLE(omnidome MACOSX_BUNDLE ${omni_src} )
  set_target_properties(omnidome PROPERTIES
               MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake-files/Info.plist")
  TARGET_LINK_LIBRARIES(omnidome omni ${omni_extra_libs} ${Qt5_LIBRARIES} )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  ADD_EXECUTABLE(omnidome ${omni_src} )
  TARGET_LINK_LIBRARIES(omnidome omni ${omni_extra_libs} ${Qt5_LIBRARIES} )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
