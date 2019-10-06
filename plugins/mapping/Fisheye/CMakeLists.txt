
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Fisheye.h
        Fisheye.cpp
        FisheyeWidget.h
        FisheyeWidget.cpp
        Fisheye.qrc
)

omni_plugin(plugin_mapping_Fisheye "${SOURCES}")

