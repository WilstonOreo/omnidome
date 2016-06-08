# Build target options for syphon
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(SYPHON_FRAMEWORK_DEST "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/omnidome.app/Contents/Frameworks/Syphon.framework")

  add_custom_command(TARGET plugin_input_Syphon PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory
                 ${SYPHON_FRAMEWORK_DEST})
  add_custom_command(TARGET plugin_input_Syphon POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                 "${plugin_input_Syphon_PATH}/Syphon.framework"
                 "${SYPHON_FRAMEWORK_DEST}")
ENDIF()
