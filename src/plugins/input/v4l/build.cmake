MESSAGE("Build setup for V4L plugin...")

IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# Plugin only works on Linux
 SET(plugin_input_v4l_IGNORE TRUE)
 return()
ENDIF()

find_library(LIBV4L2 v4l2)
find_library(LIBUDEV udev)
find_library(LIBSWSCALE swscale)

SET(plugin_input_v4l_LIBRARIES ${LIBV4L2} ${LIBUDEV} ${LIBSWSCALE})
