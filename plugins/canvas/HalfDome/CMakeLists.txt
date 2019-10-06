
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        HalfDome.h
        HalfDome.cpp
        HalfDomeWidget.h
        HalfDomeWidget.cpp
)

omni_plugin(plugin_canvas_HalfDome "${SOURCES}")

