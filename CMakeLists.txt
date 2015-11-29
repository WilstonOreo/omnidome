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
)

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )


################################
#set(QT_PATH Test/Qt)
detect_qt()
#setup_qt(5.5 ${CMAKE_SOURCE_DIR}/../Qt )

if(NOT QT_FOUND)
    MESSAGE(FATAL_ERROR "Qt Framework was not found. You might set QT_PATH manually.")
    return()
endif()


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

########### BUILD_TARGETS ############

FILE(GLOB_RECURSE omni_cpp_files ${CMAKE_SOURCE_DIR}/src/lib/*.cpp )

ADD_LIBRARY(omni STATIC ${omni_cpp_files})
TARGET_LINK_LIBRARIES(omni ${Qt5_LIBRARIES})

FILE(GLOB_RECURSE omni_app_cpp_files
  ${CMAKE_SOURCE_DIR}/src/slim/*.cpp
  ${CMAKE_SOURCE_DIR}/src/app/*.cpp
  )
list(REMOVE_ITEM omni_app_cpp_files "${CMAKE_SOURCE_DIR}/src/app/omnidome.cpp")

FILE(GLOB_RECURSE omni_headers ${CMAKE_SOURCE_DIR}/include/omni/ui/*.h ${CMAKE_SOURCE_DIR}/include/slim/*.h )
FILE(GLOB_RECURSE omni_forms ${CMAKE_SOURCE_DIR}/src/app/ui/*.ui )
FILE(GLOB_RECURSE omni_resources ${CMAKE_SOURCE_DIR}/src/app/rc/*.qrc )

common_cxx_flags()


########### Plugins ############

#include(src/plugins/input/gstreamer/gstreamer.cmake)



QT5_WRAP_CPP(omni_headers_moc ${omni_headers})
QT5_WRAP_CPP(slim_widgetz_headers_moc ${slim_widgetz_header_files})
QT5_ADD_RESOURCES(omni_resources_rcc ${omni_resources})
QT5_GENERATE_UI(omni_forms_headers ${omni_forms})
SET(omni_src ${omni_app_cpp_files} ${omni_headers_moc} ${omni_forms_headers} ${omni_resources_rcc})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(omni_extra_libs OpenGL )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(omni_extra_libs GL )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


########### Tests #############

if(DEFINED OMNI_GSTREAMER)
  omni_gstreamer_test()
endif()

ADD_EXECUTABLE(test_Projector
  ./src/test/Projector.cpp
  ${omni_src} )

TARGET_LINK_LIBRARIES(test_Projector ${Qt5_LIBRARIES} ${omni_extra_libs} omni )


ADD_EXECUTABLE(test_GL ./src/test/GL.cpp  ${omni_src})
TARGET_LINK_LIBRARIES(test_GL ${Qt5_LIBRARIES} ${omni_extra_libs} omni )

ADD_EXECUTABLE(test_WarpGL ./src/test/WarpGL.cpp ${omni_src} )
TARGET_LINK_LIBRARIES(test_WarpGL ${Qt5_LIBRARIES}  ${omni_extra_libs} omni  )

ADD_EXECUTABLE(test_Serialization
  ./src/test/Serialization.cpp
  ${omni_src} )
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

  ADD_EXECUTABLE(omnidome MACOSX_BUNDLE ${CMAKE_SOURCE_DIR}/src/app/omnidome.cpp ${omni_src} )
  set_target_properties(omnidome PROPERTIES
               MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake-files/Info.plist")
  TARGET_LINK_LIBRARIES(omnidome omni ${omni_extra_libs} ${Qt5_LIBRARIES} )


ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  ADD_EXECUTABLE(omnidome ./src/app/omnidome.cpp ${omni_src} )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
