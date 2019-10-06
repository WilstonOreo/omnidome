
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        CubeMap.h
        CubeMap.cpp
        CubeMapWidget.h
        CubeMapWidget.cpp
        CubeMap.qrc
)

omni_plugin(plugin_mapping_CubeMap "${SOURCES}")

