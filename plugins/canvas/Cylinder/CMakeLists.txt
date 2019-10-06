
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Cylinder.h
        Cylinder.cpp
        CylinderWidget.h
        CylinderWidget.cpp
)

omni_plugin(plugin_canvas_Cylinder "${SOURCES}")

