
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(SOURCES
        TestImage.h
        TestImage.cpp
        TestImageWidget.h
        TestImageWidget.cpp
        TestInputPreview.h
        TestInputPreview.cpp
        TestImage.qrc
)

omni_plugin(plugin_input_TestImage "${SOURCES}")

