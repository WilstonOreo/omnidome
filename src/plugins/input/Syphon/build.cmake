MESSAGE("Build setup for syphon plugin...")

IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# Plugin only works on Mac OS X
 SET(plugin_input_Syphon_IGNORE TRUE)
 return()
ENDIF()

FIND_PATH(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)

find_library(
  SYPHON Syphon PATHS ${plugin_input_Syphon_PATH}
  NO_DEFAULT_PATH
)

include_directories(${plugin_input_Syphon_PATH}/Syphon.framework)

find_library(COCOA Cocoa)
find_library(CORE_FOUNDATION CoreFoundation)


SET(plugin_input_Syphon_FRAMEWORKS ${SYPHON} ${COCOA} ${CORE_FOUNDATION})
MARK_AS_ADVANCED(${SYPHON} ${COCOA} ${CORE_FOUNDATION})

# Copy Syphon framework file into omnidome app folder
