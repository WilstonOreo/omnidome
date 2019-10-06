
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        FullSphere.h
        FullSphere.cpp
        FullSphereWidget.h
        FullSphereWidget.cpp
)

omni_plugin(plugin_canvas_FullSphere "${SOURCES}")

