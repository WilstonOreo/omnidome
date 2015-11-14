    
cmake_minimum_required(VERSION 3.2)
set(CMAKE_OSX_ARCHITECTURES "i386")

PROJECT(OmnidomeFFGLPlugin)


set(OMNI_FFGL TRUE)

set(OMNI_FFGL_PATH 
  ${CMAKE_SOURCE_DIR}
)

include_directories(${OMNI_FFGL_PATH})

set(OMNI_FFGL_COMMON_CPP_FILES 
  ${OMNI_FFGL_PATH}/src/FFGLPluginInfo.cpp
  ${OMNI_FFGL_PATH}/src/FFGLPluginInfoData.cpp
  ${OMNI_FFGL_PATH}/src/FFGLPluginManager.cpp
  ${OMNI_FFGL_PATH}/src/FFGLPluginSDK.cpp
  ${OMNI_FFGL_PATH}/src/FFGL.cpp
  ${OMNI_FFGL_PATH}/src/FFGLShader.cpp
  ${OMNI_FFGL_PATH}/src/FFGLFBO.cpp
  ${OMNI_FFGL_PATH}/src/FFGLExtensions.cpp
)


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  ADD_EXECUTABLE(FFGLOmniUVW MACOSX_BUNDLE 
    ${OMNI_FFGL_COMMON_CPP_FILES} 
    FFGLOmniUVW/FFGLOmniUVW.cpp
  )

  SET(MACOSX_BUNDLE_INFO_STRING "Omnidome FFGL Plugin for UVW maps")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "com.resolume.omnidome")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version 1.0.0")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "Omnidome FFGLPlugin")
  SET(MACOSX_BUNDLE_BUNDLE_PACKAGE_TYPE "BNDL")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0")
  SET(MACOSX_BUNDLE_COPYRIGHT "(C) 2015, Michael Winkelmann, cr8tr.org, omnido.me")

  SET_TARGET_PROPERTIES( FFGLOmniUVW
         PROPERTIES
         IMPORT_SUFFIX ""
         IMPORT_PREFIX ""
         PREFIX ""
         SUFFIX ""
     )

  ADD_DEFINITIONS( -fPIC -DTARGET_OS_MAC -O3  -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 )
  ADD_DEFINITIONS(-ferror-limit=5 -fcolor-diagnostics -fdiagnostics-show-template-tree  -Wno-deprecated )

  set_target_properties( FFGLOmniUVW 
    PROPERTIES 
    COMPILE_FLAGS "-m32" LINK_FLAGS "-bundle -m32 -framework OpenGL")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  TARGET_LINK_LIBRARIES( -lGL ) 
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


