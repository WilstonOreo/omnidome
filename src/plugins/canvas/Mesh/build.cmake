omni_option(
    OpenMesh_PATH "Path to OpenMesh source" 
    ${CMAKE_SOURCE_DIR}/OpenMesh
)

set(OpenMesh_LIBRARY_PATH ${OpenMesh_PATH}/Build/Build/lib)
set(OpenMesh_INCLUDE_PATH ${OpenMesh_PATH}/src)
link_directories(${OpenMesh_LIBRARY_PATH})
include_directories(${OpenMesh_INCLUDE_PATH})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OpenMesh_CORE_LIB ${OpenMesh_LIBRARY_PATH}/libOpenMeshCore.dylib)
  set(OpenMesh_TOOLS_LIB ${OpenMesh_LIBRARY_PATH}/libOpenMeshTools.dylib)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(OpenMesh_CORE_LIB ${OpenMesh_LIBRARY_PATH}/libOpenMeshCore.so)
  set(OpenMesh_TOOLS_LIB ${OpenMesh_LIBRARY_PATH}/libOpenMeshTools.so)
ENDIF()


SET(plugin_canvas_Mesh_LIBRARIES ${OpenMesh_CORE_LIB} ${OpenMesh_TOOLS_LIB})
