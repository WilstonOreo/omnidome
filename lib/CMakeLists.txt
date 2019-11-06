

file(GLOB HEADERS_PUB_ROOT "include/omni/*.h")
file(GLOB HEADERS_PUB_SUB "include/omni/**/*.h")

find_package(Qt5 COMPONENTS Core Gui REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCES
        ${HEADERS_PUB_ROOT}
        ${HEADERS_PUB_SUB}
        src/BlendMask.cpp
        src/BlendBrush.cpp
        src/BlendSettings.cpp
        src/canvas/Dome.cpp
        src/canvas/Envelope.cpp
        src/canvas/Interface.cpp
        src/LengthUnit.cpp
        src/PluginInfo.cpp
        src/PluginLoader.cpp
        src/geometry/AffineTransform.cpp
        src/geometry/Angle.cpp
        src/geometry/Box.cpp
        src/geometry/EulerAngles.cpp
        src/geometry/PolarVec.cpp
        src/geometry/Vertex.cpp
        src/geometry/Vertex2D.cpp
        src/input/List.cpp
        src/input/Interface.cpp
        src/input/Framebuffer.cpp
        src/mapping/Interface.cpp
        src/proj/Calibration.cpp
        src/proj/Calibrator.cpp
        src/proj/ChannelCorrection.cpp
        src/proj/ColorCorrection.cpp
        src/proj/Frustum.cpp
        src/proj/Projector.cpp
        src/proj/ScreenSetup.cpp
        src/proj/Tuning.cpp
        src/proj/TuningList.cpp
        src/render/ExportSettings.cpp
        src/render/Renderer.cpp
        src/serialization/PropertyMap.cpp
        src/visual/BoundingBox.cpp
        src/visual/Box.cpp
        src/visual/CameraInterface.cpp
        src/visual/Circle.cpp
        src/visual/ContextSwitch.cpp
        src/visual/Grid.cpp
        src/visual/Interface.cpp
        src/visual/Light.cpp
        src/visual/PerspectiveCamera.cpp
        src/visual/Plane.cpp
        src/visual/Projector.cpp
        src/visual/Rectangle.cpp
        src/visual/Scene.cpp
        src/visual/Session.cpp
        src/visual/Shader.cpp
        src/visual/Sphere.cpp
        src/visual/Tracker.cpp
        src/visual/Tuning.cpp
        src/visual/UniformHandler.cpp
        src/visual/util.cpp
        src/visual/VBO.cpp
        src/visual/VertexVBO.cpp
        src/visual/WarpGrid.cpp
        src/media/Location.cpp
        src/Session.cpp
        src/util.cpp
        src/WarpGrid.cpp
        src/WarpPoint.cpp
        src/libomni.qrc
)

add_library(omni SHARED ${SOURCES})
target_include_directories(omni
    PUBLIC
        include
    PRIVATE
        src
)
target_link_libraries(omni
    PUBLIC
        Qt5::Core
        Qt5::Gui
        ${OPENGL_LIBRARIES}
)

target_compile_definitions(omni
    PUBLIC
        OMNI_LIBRARY)

if(WIN32)
    SET(OUTPUT_PATH ../app)

    SET_TARGET_PROPERTIES(omni
          PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}
          LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}
      )
endif()
