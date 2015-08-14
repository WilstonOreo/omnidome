# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(Omnidome)

include(cmake-files/base.cmake)

import(
  compiler_flags
  doxygen
  qt
  cpack
)

########### BUILD_TARGETS ############

FILE(GLOB_RECURSE omni_cpp_files ${CMAKE_SOURCE_DIR}/src/lib/*.cpp )


ADD_LIBRARY(omni STATIC ${omni_cpp_files})
TARGET_LINK_LIBRARIES(omni ${QT5_LIBRARIES})

FILE(GLOB_RECURSE omni_app_cpp_files 
  ${CMAKE_SOURCE_DIR}/src/slim/*.cpp 
  ${CMAKE_SOURCE_DIR}/src/app/*.cpp 
  )
list(REMOVE_ITEM omni_app_cpp_files "${CMAKE_SOURCE_DIR}/src/app/omnidome.cpp")

FILE(GLOB_RECURSE omni_headers ${CMAKE_SOURCE_DIR}/include/omni/ui/*.h ${CMAKE_SOURCE_DIR}/include/slim/*.h )
FILE(GLOB_RECURSE omni_forms ${CMAKE_SOURCE_DIR}/src/app/ui/*.ui )
FILE(GLOB_RECURSE omni_resources ${CMAKE_SOURCE_DIR}/src/app/rc/*.qrc )

common_cxx_flags()

########### Plugins ############

#include(src/plugins/gstreamer/gstreamer.cmake)

################################

setup_qt(5 4)
QT5_WRAP_CPP(omni_headers_moc ${omni_headers})
QT5_WRAP_CPP(slim_widgetz_headers_moc ${slim_widgetz_header_files})
QT5_ADD_RESOURCES(omni_resources_rcc ${omni_resources})
QT5_GENERATE_UI(omni_forms_headers ${omni_forms})
SET(omni_src ${omni_app_cpp_files} ${omni_headers_moc} ${omni_forms_headers} ${omni_resources_rcc})
  
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(omni_extra_libs OpenGL )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(omni_extra_libs GL )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


########### Tests #############

if(DEFINED OMNI_GSTREAMER)
  omni_gstreamer_test()
endif()


message(STATUS ${omni_extra_libs})
ADD_EXECUTABLE(test_Projector
  ./src/test/Projector.cpp 
  ${omni_src} )

TARGET_LINK_LIBRARIES(test_Projector ${Qt5_LIBRARIES} ${omni_extra_libs} omni )


ADD_EXECUTABLE(test_GL
  ./src/test/GL.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_GL ${Qt5_LIBRARIES} ${omni_extra_libs} omni )

ADD_EXECUTABLE(test_WarpGL
  ./src/test/WarpGL.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_WarpGL ${Qt5_LIBRARIES}  ${omni_extra_libs} omni  )

ADD_EXECUTABLE(test_Serialization
  ./src/test/Serialization.cpp 
  ${omni_src} )
TARGET_LINK_LIBRARIES(test_Serialization ${Qt5_LIBRARIES} ${omni_extra_libs}  omni )


# Main Application
ADD_EXECUTABLE(omnidome ./src/app/omnidome.cpp ${omni_src} )
TARGET_LINK_LIBRARIES(omnidome omni ${omni_extra_libs} ${Qt5_LIBRARIES} )

