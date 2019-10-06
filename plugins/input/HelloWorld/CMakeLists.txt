
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        HelloWorld.h
        HelloWorld.cpp
        HelloWorld.qrc
)

omni_plugin(plugin_input_HelloWorld "${SOURCES}")

