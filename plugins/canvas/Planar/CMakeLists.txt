
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Planar.h
        Planar.cpp
        PlanarWidget.h
        PlanarWidget.cpp
)

omni_plugin(plugin_canvas_Planar "${SOURCES}")

