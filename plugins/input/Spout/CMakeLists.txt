if (NOT WIN32)
    return()
endif()

include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

set(Spout_PATH SpoutSDK)

set(Spout_SOURCES
    ${Spout_PATH}/Spout.h
    ${Spout_PATH}/SpoutCommon.h
    ${Spout_PATH}/SpoutCopy.h
    ${Spout_PATH}/SpoutCopy.cpp
    ${Spout_PATH}/SpoutDirectX.h
    ${Spout_PATH}/SpoutDirectX.cpp
    ${Spout_PATH}/SpoutGLDXinterop.h
    ${Spout_PATH}/SpoutGLDXinterop.cpp
    ${Spout_PATH}/SpoutGLextensions.h
    ${Spout_PATH}/SpoutGLextensions.cpp
    ${Spout_PATH}/SpoutMemoryShare.h
    ${Spout_PATH}/SpoutMemoryShare.cpp
    ${Spout_PATH}/SpoutReceiver.h
    ${Spout_PATH}/SpoutReceiver.cpp
    ${Spout_PATH}/SpoutSDK.h
    ${Spout_PATH}/SpoutSDK.cpp
    ${Spout_PATH}/SpoutSenderNames.h
    ${Spout_PATH}/SpoutSenderNames.cpp
    ${Spout_PATH}/SpoutSharedMemory.h
    ${Spout_PATH}/SpoutSharedMemory.cpp
    )

set(SOURCES
        Spout.h
        Spout.cpp
     #   SpoutWidget.h
     #   SpoutWidget.cpp
        ${Spout_SOURCES}
)

omni_plugin(plugin_input_Spout "${SOURCES}")

target_include_directories(plugin_input_Spout
    PUBLIC
    ${Spout_PATH})

target_link_libraries(plugin_input_Spout
    PUBLIC
    d3d9 d3d11 d3dx11 dxgi shlwapi version)

target_compile_options(plugin_input_Spout
    PUBLIC
     -msse4)
