
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Equirectangular.h
        Equirectangular.cpp
        EquirectangularWidget.h
        EquirectangularWidget.cpp
        Equirectangular.qrc
)

omni_plugin(plugin_mapping_Equirectangular "${SOURCES}")

