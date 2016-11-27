# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# project id
PROJECT(Omnidome)

# Initialize omni build system system
SET(OMNI_SUMMARY "OMNIDOME is a arbitrary surface projection mapping tool. It allows you to map any physical room and project virtual content back on to it.")
SET(OMNI_VENDOR "Michael Winkelmann")
SET(OMNI_VENDOR_SHORT "omnidome")
SET(OMNI_VENDOR_EMAIL "michael@winkelmann.site")
SET(OMNI_URL "omnido.me")

include(cmake-scripts/base.cmake)

# Use Qt GL binding for OmniCalibration SDK
add_definitions(-DOMNIC_USE_QT_GL)
#omni_set_version_for_project(1 0 0 OmniCalibration)
omni_add_submodule(omnicalib)

# Import omni modules
omni_import(
  doxygen
  gl
  cpack
  plugin
  qt
)

omni_set_version(1 0 1)

# Setup libomni build target
SET(omni_LIBRARIES ${Qt5_LIBRARIES} ${GL_LIBRARIES})
SET(omni_HEADERS_MOC_PATH ${CMAKE_SOURCE_DIR}/include/omni/ui )
omni_add_shared_library(omni ${OMNI_LIBRARY_SOURCE_PATH})

# Load canvas plugins
SET(OMNI_PLUGIN_EXTENSION "omnix")
SET(OMNI_PLUGIN_DEFAULT_LIBRARIES ${Qt5_LIBRARIES} ${GL_LIBRARIES} omni)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
  # Put plugins in plugins folder in omnidome.app dir
  set(OMNI_PLUGIN_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/omnidome.app/Contents/PlugIns)
ENDIF()

omni_load_plugins(${OMNI_PLUGIN_PATH}/canvas)

# Load input plugins
omni_load_plugins(${OMNI_PLUGIN_PATH}/input)

# Load mapping inputs
omni_load_plugins(${OMNI_PLUGIN_PATH}/mapping)

# Setup omnidome executable
SET(omnidome_LIBRARIES ${Qt5_LIBRARIES} ${GL_LIBRARIES} omni)
SET(omnidome_HEADERS_MOC_PATH ${OMNI_MAINAPP_SOURCE_PATH} )
omni_mainapp(omnidome)

# Initialize Packaging with CPack
omni_setup_cpack()
