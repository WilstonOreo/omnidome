
include(${CMAKE_SOURCE_DIR}/cmake/plugin.cmake)

file(GLOB_RECURSE OpenMesh_SOURCES "OpenMesh/*.cc")

set(SOURCES
        Mesh.h
        Mesh.cpp
        MeshWidget.h
        MeshWidget.cpp
        ${OpenMesh_SOURCES}
)

set(OpenMesh_PATH ${CMAKE_SOURCE_DIR}/OpenMesh)

omni_plugin(plugin_canvas_Mesh "${SOURCES}")
