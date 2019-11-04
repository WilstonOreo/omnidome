
file(GLOB HEADERS_PUB_ROOT "include/omni/*.h")
file(GLOB HEADERS_PUB_SUB "include/omni/**/*.h")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
        ${HEADERS_PUB_ROOT}
        ${HEADERS_PUB_SUB}
        src/AbstractInputWidget.cpp
        src/AffineTransform.cpp
        src/CanvasParameters.cpp
        src/ColorSet.cpp
        src/Dial.cpp
        src/ExceptionList.cpp
        src/GLView.cpp
        src/InputPreview.cpp
        src/MappingParameters.cpp
        src/ParameterWidget.cpp
        src/RangedFloat.cpp
        src/RangedInt.cpp
        src/Rotation.cpp
        src/Widget.cpp
)

add_library(omniwidget SHARED ${SOURCES})
target_include_directories(omniwidget
    PUBLIC
        include
        ../../lib/include
    PRIVATE
        src
)
target_link_libraries(omniwidget
    PUBLIC
        omni
        Qt5::Widgets
)

target_compile_definitions(omniwidget
    PUBLIC
        OMNIWIDGET_LIBRARY)

if(WIN32)
  SET(OUTPUT_PATH ../app)

  SET_TARGET_PROPERTIES(omniwidget
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}
      LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}
  )
endif()
