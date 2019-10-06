
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Box.h
        Box.cpp
        BoxWidget.h
        BoxWidget.cpp
)

omni_plugin(plugin_canvas_Box "${SOURCES}")

