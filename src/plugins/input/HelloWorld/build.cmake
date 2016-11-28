# Plugin is only build in Debug mode
IF(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
  SET(plugin_input_HelloWorld_IGNORE TRUE)
  return()
ENDIF()
