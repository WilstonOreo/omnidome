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
  cpack
)

add_submodule(slim-widgetz)
add_submodule(boostx-factory)

FILE(GLOB_RECURSE omni_cpp_files ${CMAKE_SOURCE_DIR}/src/lib/*.cpp )

ADD_LIBRARY(omni STATIC ${omni_cpp_files})
TARGET_LINK_LIBRARIES(omni ${QT5_LIBRARIES})

SET(omni_gui_cpp_files
    ./src/app/MainWindow.cpp 
    ./src/app/proj/TuningList.cpp 
    ./src/app/proj/Tuning.cpp 
    ./src/app/proj/TitleBar.cpp
    ./src/app/ScreenSetup.cpp 
    ./src/app/GLView.cpp
    ./src/app/GLView3D.cpp
    ./src/app/TuningGLView.cpp
    ./src/app/About.cpp
    ./src/app/AboutGL.cpp
    ./src/app/Export.cpp
    ./src/app/Canvas.cpp
    ./src/app/CanvasParameters.cpp
    ./src/app/Mapping.cpp
    ./src/app/MappingParameters.cpp
    ./src/app/Input.cpp
    ./src/app/Blend.cpp
    ./src/app/BrushPreview.cpp
    ./src/app/ParameterWidget.cpp
    ./src/app/Warp.cpp
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
        include/omni/ui/TuningGLView.h
        include/omni/ui/GLView3D.h 
        include/omni/ui/ParameterWidget.h 
        include/omni/ui/Export.h 
        include/omni/ui/Canvas.h 
        include/omni/ui/CanvasParameters.h 
        include/omni/ui/Mapping.h 
        include/omni/ui/MappingParameters.h 
        include/omni/ui/Input.h 
        include/omni/ui/Blend.h 
        include/omni/ui/Warp.h 
        include/omni/ui/BrushPreview.h 
  )

FILE(GLOB_RECURSE omni_forms ${CMAKE_SOURCE_DIR}/src/app/ui/*.ui )
FILE(GLOB_RECURSE omni_resources ${CMAKE_SOURCE_DIR}/src/app/rc/*.qrc )

common_cxx_flags()

########### Plugins ############

include(src/plugins/gstreamer/gstreamer.cmake)

################################

setup_qt(5 4)
QT5_WRAP_CPP(omni_headers_moc ${omni_headers})
QT5_ADD_RESOURCES(omni_resources_rcc ${omni_resources})
QT5_GENERATE_UI(omni_forms_headers ${omni_forms})
SET(omni_src ${omni_gui_cpp_files} ${omni_headers_moc} ${omni_forms_headers} ${omni_resources_rcc})

set(omni_extra_libs GL )



########### Tests #############

if(DEFINED OMNI_GSTREAMER)
  omni_gstreamer_test()
endif()


message(STATUS ${omni_extra_libs})
ADD_EXECUTABLE(test_Projector
  ./src/test/Projector.cpp 
  ${omni_src} )

TARGET_LINK_LIBRARIES(test_Projector ${QT_LIBRARIES} ${omni_extra_libs} slim-widgetz omni )


ADD_EXECUTABLE(test_ScreenSetup
  ./src/test/ScreenSetup.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_ScreenSetup ${QT5_LIBRARIES} ${omni_extra_libs} slim-widgetz omni )

ADD_EXECUTABLE(test_GL
  ./src/test/GL.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_GL ${QT5_LIBRARIES} ${omni_extra_libs} slim-widgetz omni )

ADD_EXECUTABLE(test_Serialization
  ./src/test/Serialization.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_Serialization ${QT5_LIBRARIES} ${omni_extra_libs} slim-widgetz omni )


# Main Application
ADD_EXECUTABLE(omnidome ./src/app/omnidome.cpp ${omni_src} )
TARGET_LINK_LIBRARIES(omnidome omni slim-widgetz ${omni_extra_libs} ${Qt5_LIBRARIES} )

