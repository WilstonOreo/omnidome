# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

# project id
PROJECT(Omnidome)

# Initialize omni build system system
SET(OMNI_SUMMARY "OMNIDOME is a arbitrary surface projection mapping tool. It allows you to map any physical room and project virtual content back on to it.")
SET(OMNI_VENDOR "Michael Winkelmann")
SET(OMNI_VENDOR_SHORT "omnidome")
SET(OMNI_VENDOR_EMAIL "michael@winkelmann.site")
SET(OMNI_URL "omnido.me")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_SOURCE_DIR}/cmake/version.cmake)

add_definitions(-DGL_SILENCE_DEPRECATION)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set( CMAKE_MACOSX_RPATH 1)

      # use, i.e. don't skip the full RPATH for the build tree
      SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

      # when building, don't use the install RPATH already
      # (but later on when installing)
      SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

      SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

      # the RPATH to be used when installing, but only if it's not a system directory
      LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
      IF("${isSystemDir}" STREQUAL "-1")
          SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
      ENDIF("${isSystemDir}" STREQUAL "-1")
ENDIF()

omni_set_version(1 1 0)

add_subdirectory(lib)
add_subdirectory(widgetlib)
add_subdirectory(app)
add_subdirectory(plugins)
add_subdirectory(test)


