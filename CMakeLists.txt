# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# project id
PROJECT(Omnidome)

# Initialize cm8kr build system system
SET(CM8KR_SUMMARY "OMNIDOME is a arbitrary surface projection mapping tool built by CR8TR. It allows you to map any physical room and project virtual content back on to it.")
SET(CM8KR_VENDOR "CR8TR GmbH")
SET(CM8KR_VENDOR_SHORT "cr8tr")
SET(CM8KR_VENDOR_EMAIL "office@cr8tr.org")
SET(CM8KR_URL "omnido.me / cr8tr.org")

include(../cm8kr/base.cmake)

# Import cm8kr modules
cm8kr_import(
  doxygen
  gl
  cpack
  plugin
  qt
)

include(../omnicalib/build.cmake)


cm8kr_set_version(1 0 1)

# Setup libomni build target
SET(omni_LIBRARIES ${Qt5_LIBRARIES} ${GL_LIBRARIES})
SET(omni_HEADERS_MOC_PATH ${CMAKE_SOURCE_DIR}/include/omni/ui )
cm8kr_add_shared_library(omni ${CM8KR_LIBRARY_SOURCE_PATH})

# Load canvas plugins
SET(CM8KR_PLUGIN_EXTENSION "omnix")
SET(CM8KR_PLUGIN_DEFAULT_LIBRARIES ${Qt5_LIBRARIES} ${GL_LIBRARIES} omni)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
  # Put plugins in plugins folder in omnidome.app dir
  set(CM8KR_PLUGIN_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/omnidome.app/Contents/PlugIns)
ENDIF()

cm8kr_load_plugins(${CM8KR_PLUGIN_PATH}/canvas)

# Load input plugins
cm8kr_load_plugins(${CM8KR_PLUGIN_PATH}/input)

# Load mapping inputs
cm8kr_load_plugins(${CM8KR_PLUGIN_PATH}/mapping)

# Setup omnidome executable
SET(omnidome_LIBRARIES ${Qt5_LIBRARIES} ${GL_LIBRARIES} omni)
SET(omnidome_HEADERS_MOC_PATH ${CM8KR_MAINAPP_SOURCE_PATH} )
cm8kr_mainapp(omnidome)

# Initialize Packaging with CPack
cm8kr_setup_cpack()
