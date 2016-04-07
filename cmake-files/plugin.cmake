
function(build_plugin PLUGIN_PREFIX PLUGIN_DIR)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
      SET(OMNIDOME_BUILD_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/omnidome.app)
    ENDIF()


    get_filename_component(PLUGIN_NAME ${PLUGIN_DIR} NAME)
    SET(build_target plugin_${PLUGIN_PREFIX}_${PLUGIN_NAME})

    SET(${build_target}_PATH ${PLUGIN_DIR})

    if(EXISTS ${PLUGIN_DIR}/build.cmake)
        include(${PLUGIN_DIR}/build.cmake)
    endif()

    MESSAGE(STATUS "Plugin ${build_target} in ${PLUGIN_DIR}")

    if (plugin_input_Syphon_IGNORE)
      MESSAGE(STATUS "${build_target} will be ignored.")
      return()
    endif()


    FILE(GLOB plugin_headers ${plugin_dir}/*.h )
    FILE(GLOB plugin_sources ${plugin_dir}/*.cpp )
    FILE(GLOB plugin_forms ${plugin_dir}/*.ui )
    FILE(GLOB plugin_resources ${plugin_dir}/*.qrc )

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
      # Look for ObjectiveC sources
      FILE(GLOB_RECURSE plugin_objc_sources ${plugin_dir}/*.m )
      if (plugin_objc_sources)
        set_source_files_properties(${plugin_objc_sources} PROPERTIES
                                  COMPILE_FLAGS "-x objective-c")
        MESSAGE(STATUS "Objective-C sources: ${plugin_objc_sources}")
        SET(plugin_sources ${plugin_sources} ${plugin_objc_sources})
      endif()

      # Look for ObjectiveC++ sources
      FILE(GLOB_RECURSE plugin_objcpp_sources ${plugin_dir}/*.mm )
      if (plugin_objcpp_sources)
        set_source_files_properties(${plugin_objcpp_sources} PROPERTIES
                                  COMPILE_FLAGS "-x objective-c++")
        MESSAGE(STATUS "Objective-C++ sources: ${plugin_objcpp_sources}")
        SET(plugin_sources ${plugin_sources} ${plugin_objcpp_sources})
      endif()


    ENDIF()

    QT5_WRAP_CPP(plugin_headers_moc ${plugin_headers})
    QT5_ADD_RESOURCES(plugin_resources_rcc ${plugin_resources})
    QT5_GENERATE_UI(plugin_forms_headers ${plugin_forms})

    SET(plugin_src
        ${plugin_sources}
        ${plugin_objcpp_sources}
        ${plugin_objc_sources}
        ${plugin_headers_moc}
        ${plugin_forms_headers}
        ${plugin_resources_rcc})


    include_directories(${${build_target}_INCLUDE})

    add_library(${build_target} SHARED ${plugin_src})
    target_link_libraries(${build_target} ${Qt5_LIBRARIES} ${${build_target}_LIBS} omni ${omni_gl_LIBRARIES})

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
      # LINK FRAMEWORDS
      if (${build_target}_FRAMEWORKS)
        target_link_libraries(${build_target} ${${build_target}_FRAMEWORKS} )
      endif()
    ENDIF()

    target_compile_definitions(${build_target} PRIVATE ${defs})
    target_compile_definitions(${build_target} PRIVATE ${${build_target}_DEFS})

    add_definitions(${defs} ${${build_target}_DEFS})

    set(PLUGIN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If mac os x
      # Put plugins in plugins folder in omnidome.app dir
      set(PLUGIN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/omnidome.app/Contents/PlugIns)
    ENDIF()

    SET_TARGET_PROPERTIES(${build_target}
        PROPERTIES
        SUFFIX ".omnix"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
    )
endfunction()

macro(load_plugins PLUGIN_FOLDER)
    file(GLOB plugin_dirs "${PLUGIN_FOLDER}/*")

    get_filename_component(PLUGIN_PREFIX ${PLUGIN_FOLDER} NAME)
    MESSAGE(STATUS ${PLUGIN_PREFIX})

    foreach(plugin_dir ${plugin_dirs})
        IF(IS_DIRECTORY ${plugin_dir})
            build_plugin("${PLUGIN_PREFIX}" "${plugin_dir}")
        ENDIF()
    endforeach()

endmacro(load_plugins PLUGIN_DIR)
