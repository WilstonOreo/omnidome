
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Image.h
        Image.cpp
)

omni_plugin(plugin_input_Image "${SOURCES}")
