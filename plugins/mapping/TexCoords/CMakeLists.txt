
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        TexCoords.h
        TexCoords.cpp
        TexCoordsWidget.h
        TexCoordsWidget.cpp
        TexCoords.qrc
)

omni_plugin(plugin_mapping_TexCoords "${SOURCES}")

