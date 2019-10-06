
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Cylindrical.h
        Cylindrical.cpp
        CylindricalWidget.h
        CylindricalWidget.cpp
        Cylindrical.qrc
)

omni_plugin(plugin_mapping_Cylindrical "${SOURCES}")

