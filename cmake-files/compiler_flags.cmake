
MACRO(set_version_string dir)
  SET(version_script "${dir}/generate-version.sh")
  IF(EXISTS "${version_script}")
    execute_process(COMMAND $ENV{SHELL} ${version_script}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE BUILD_VERSION)
    STRING(STRIP "${BUILD_VERSION}" BUILD_VERSION)
    ADD_DEFINITIONS(-D__GITVERSIONSTRING__="${BUILD_VERSION}")
    MESSAGE(STATUS "Build version is ${BUILD_VERSION}")
  ENDIF(EXISTS "${version_script}")
ENDMACRO(set_version_string dir)

MACRO(common_cxx_flags)
  ADD_DEFINITIONS(-fPIC -fsigned-char -Wall -Wno-missing-braces)

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    ADD_DEFINITIONS("-g -DDEBUG -O1")
  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

  IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
    ADD_DEFINITIONS("-O3")

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Set clang as standard compiler
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
    ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Release")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    ADD_DEFINITIONS(-g -Os -fsigned-char -Wall -Wno-unknown-pragmas)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64 -Wno-unused-variable -DQT_SVG_LIB -DQT_XML_LIB -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_HAVE_SSE -DQT_HAVE_MMXEXT -DQT_HAVE_SSE2 -DQT_SHARED -Wno-unknown-pragmas ")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wno-unused-variable -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64 ")
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  IF (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_DEFINITIONS(-ferror-limit=5 -fcolor-diagnostics -fdiagnostics-show-template-tree  -Wno-deprecated )
  ENDIF()
ENDMACRO(common_cxx_flags)
