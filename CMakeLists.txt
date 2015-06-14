# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(Omnidome)

include(../cmake-macros/base.cmake)

import(
  add_submodule
  compiler_flags
  doxygen
  qt
  gl
  cpack
)

add_submodule(slim-widgetz)
add_submodule(boostx-factory)

SET(omni_cpp_files
  ./src/lib/canvas/Box.cpp
  ./src/lib/canvas/Planar.cpp
  ./src/lib/canvas/Envelope.cpp
  ./src/lib/canvas/Dome.cpp
  ./src/lib/canvas/HalfDome.cpp
  ./src/lib/canvas/InflatableDome.cpp
  ./src/lib/canvas/Radome.cpp
  ./src/lib/visual/Box.cpp
  ./src/lib/visual/Circle.cpp
  ./src/lib/visual/Sphere.cpp
  ./src/lib/visual/Interface.cpp
  ./src/lib/visual/Projector.cpp
  ./src/lib/visual/WarpGrid.cpp
  ./src/lib/visual/Tracker.cpp
  ./src/lib/visual/Light.cpp
  ./src/lib/visual/Camera.cpp
  ./src/lib/visual/Session.cpp
  ./src/lib/visual/VBO.cpp
  ./src/lib/proj/Screen.cpp
  ./src/lib/proj/ScreenSetup.cpp
  ./src/lib/proj/AngleSetup.cpp
  ./src/lib/proj/PeripheralSetup.cpp
  ./src/lib/proj/FreeSetup.cpp
  ./src/lib/proj/Projector.cpp
  ./src/lib/proj/Frustum.cpp
  ./src/lib/proj/Tuning.cpp
  ./src/lib/proj/TuningList.cpp
  ./src/lib/mapping/Interface.cpp
  ./src/lib/mapping/Rotatable.cpp
  ./src/lib/mapping/Equirectangular.cpp
  ./src/lib/mapping/Fisheye.cpp
  ./src/lib/mapping/CubeMapSingle.cpp
  ./src/lib/mapping/CubeMapSeparate.cpp
  ./src/lib/mapping/Cylindrical.cpp
  ./src/lib/mapping/Planar.cpp
  ./src/lib/input/List.cpp
  ./src/lib/input/Interface.cpp
  ./src/lib/input/Image.cpp
  ./src/lib/BlendBuffer.cpp
  ./src/lib/BlendBrush.cpp
  ./src/lib/BlendMask.cpp
  ./src/lib/Box.cpp
  ./src/lib/Angle.cpp
  ./src/lib/EulerAngles.cpp
  ./src/lib/PolarVec.cpp
  ./src/lib/Id.cpp
  ./src/lib/Vertex.cpp
  ./src/lib/Session.cpp
  ./src/lib/WarpGrid.cpp
  ./src/lib/WarpPoint.cpp
)

ADD_LIBRARY(omni STATIC ${omni_cpp_files})
TARGET_LINK_LIBRARIES(omni ${QT5_LIBRARIES})

SET(omni_gui_cpp_files
    ./src/app/MainWindow.cpp 
    ./src/app/proj/TuningList.cpp 
    ./src/app/proj/Tuning.cpp 
    ./src/app/proj/TitleBar.cpp
    ./src/app/ScreenSetup.cpp 
    ./src/app/GLView.cpp
    ./src/app/GLView2D.cpp
    ./src/app/GLView3D.cpp
    ./src/app/About.cpp
    ./src/app/AboutGL.cpp
)

  SET(omni_headers 
        include/omni/ui/MainWindow.h
        include/omni/ui/About.h
        include/omni/ui/AboutGL.h
        include/omni/ui/proj/Tuning.h 
        include/omni/ui/proj/TuningList.h 
        include/omni/ui/proj/TitleBar.h 
        include/omni/ui/ScreenSetup.h 
        include/omni/ui/GLView.h 
        include/omni/ui/GLView2D.h 
        include/omni/ui/GLView3D.h 
  )
  SET(omni_forms   
    ./src/app/ui/omni_ui_MainWindow.ui
    ./src/app/ui/omni_ui_About.ui
  )

  SET(omni_resources ./src/app/rc/style.qrc ./src/app/rc/ODM.qrc )

common_cxx_flags()

setup_qt(5 4)
QT5_WRAP_CPP(omni_headers_moc ${omni_headers})
QT5_ADD_RESOURCES(omni_resources_rcc ${omni_resources})
QT5_GENERATE_UI(omni_forms_headers ${omni_forms})
SET(omni_src ${omni_gui_cpp_files} ${omni_headers_moc} ${omni_forms_headers} ${omni_resources_rcc})

setup_gl()

set(EXTRA_LIBS GL )

# Main Application
ADD_EXECUTABLE(omnidome ./src/app/omnidome.cpp ${omni_src} )
TARGET_LINK_LIBRARIES(omnidome omni slim-widgetz ${EXTRA_LIBS} ${Qt5_LIBRARIES} )

# ISF generator tool
#ADD_EXECUTABLE(omni-isf ./src/app/omni-isf.cpp )
#TARGET_LINK_LIBRARIES(omni-isf ${Boost_LIBRARIES} ${EXTRA_LIBS} ${Qt5_LIBRARIES} )

########### Test #############

ADD_EXECUTABLE(test_Projector
  ./src/test/Projector.cpp 
  ${omni_src} )

TARGET_LINK_LIBRARIES(test_Projector ${QT_LIBRARIES} ${EXTRA_LIBS} slim-widgetz omni )

ADD_EXECUTABLE(test_ScreenSetup
  ./src/test/ScreenSetup.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_ScreenSetup ${QT5_LIBRARIES} ${EXTRA_LIBS} slim-widgetz omni )

ADD_EXECUTABLE(test_GL
  ./src/test/GL.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_GL ${QT5_LIBRARIES} ${EXTRA_LIBS} slim-widgetz omni )



