if (NOT APPLE)
    return()
endif()

include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

FIND_PATH(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)

find_library(
  SYPHON Syphon PATHS .
  NO_DEFAULT_PATH
)
find_library(COCOA Cocoa)
find_library(CORE_FOUNDATION CoreFoundation)

set(SOURCES
        Syphon.h
        Syphon.mm
        SyphonNameboundClient.h
        SyphonNameboundClient.m
        SyphonServerDescription.h
        SyphonServerDescription.cpp
        SyphonServerItemModel.h
        SyphonServerItemModel.cpp
        SyphonServerManager.h
        SyphonServerManager.mm
        SyphonWidget.h
        SyphonWidget.cpp
)

omni_plugin(plugin_input_Syphon "${SOURCES}")

target_include_directories(plugin_input_Syphon
    PUBLIC
    ./Syphon.framework)

target_link_libraries(plugin_input_Syphon
    PUBLIC
    ${SYPHON} ${COCOA} ${CORE_FOUNDATION}
    )

set_source_files_properties(SyphonNameboundClient.m PROPERTIES
    COMPILE_FLAGS "-x objective-c")

set_source_files_properties(Syphon.mm SyphonServerManager.mm PROPERTIES
    COMPILE_FLAGS "-x objective-c++")

MARK_AS_ADVANCED(${SYPHON} ${COCOA} ${CORE_FOUNDATION})

SET(SYPHON_FRAMEWORK_DEST "../../../app/omnidome.app/Contents/Frameworks/Syphon.framework")

add_custom_command(TARGET plugin_input_Syphon PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${SYPHON_FRAMEWORK_DEST})

add_custom_command(TARGET plugin_input_Syphon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_LIST_DIR}/Syphon.framework"
    "${SYPHON_FRAMEWORK_DEST}")
