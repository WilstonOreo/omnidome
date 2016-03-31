
function(build_plugin PLUGIN_PREFIX PLUGIN_DIR)
    if(${PLUGIN_DIR}/build.cmake)
        include(${PLUGIN_DIR}/build.cmake)
    endif()

    MESSAGE(STATUS "Plugin ${PLUGIN_DIR}")

    get_filename_component(PLUGIN_NAME ${PLUGIN_DIR} NAME)

    FILE(GLOB_RECURSE plugin_headers ${plugin_dir}/*.h )
    FILE(GLOB_RECURSE plugin_sources ${plugin_dir}/*.cpp )
    FILE(GLOB_RECURSE plugin_forms ${plugin_dir}/*.ui )
    FILE(GLOB_RECURSE plugin_resources ${plugin_dir}/*.qrc )

    QT5_WRAP_CPP(plugin_headers_moc ${plugin_headers})
    QT5_ADD_RESOURCES(plugin_resources_rcc ${plugin_resources})
    QT5_GENERATE_UI(plugin_forms_headers ${plugin_forms})

    SET(plugin_src
        ${plugin_sources}
        ${plugin_headers_moc}
        ${plugin_forms_headers}
        ${plugin_resources_rcc})

    SET(build_target plugin_${PLUGIN_PREFIX}_${PLUGIN_NAME})

    add_library(${build_target} SHARED ${plugin_src})
    target_link_libraries(${build_target} ${Qt5_LIBRARIES} ${${build_target}_LIBS} omni ${omni_gl_LIBRARIES})
    target_compile_definitions(${build_target} PRIVATE ${defs})
    target_compile_definitions(${build_target} PRIVATE ${${build_target}_DEFS})

    add_definitions(${defs} ${${build_target}_DEFS})

    SET_TARGET_PROPERTIES(${build_target}
        PROPERTIES
        SUFFIX ".omnix"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
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
