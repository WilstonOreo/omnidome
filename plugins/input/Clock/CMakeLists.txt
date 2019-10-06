
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        Clock.h
        Clock.cpp
        ClockWidget.h
        ClockWidget.cpp
        Clock.qrc
)

omni_plugin(plugin_input_Clock "${SOURCES}")
