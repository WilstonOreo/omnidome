
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    proj/ChannelCorrectionParameters.h
    proj/ChannelCorrectionParameters.cpp
    proj/ColorCorrectionGraph.h
    proj/ColorCorrectionGraph.cpp
    proj/TitleBar.h
    proj/TitleBar.cpp
    proj/Tuning.h
    proj/Tuning.cpp
    proj/TuningLayout.h
    proj/TuningLayout.cpp
    proj/TuningList.h
    proj/TuningList.cpp
    About.cpp
    About.h
    AboutGL.cpp
    AboutGL.h
    Application.h
    Application.cpp
    Blend.h
    Blend.cpp
    BrushPreview.h
    BrushPreview.cpp
    Canvas.h
    Canvas.cpp
    ColorCorrection.h
    ColorCorrection.cpp
    DockWidget.h
    DockWidget.cpp
    Export.h
    Export.cpp
    FullscreenTuningGLView.h
    FullscreenTuningGLView.cpp
    Input.h
    Input.cpp
    MainWindow.h
    MainWindow.cpp
    Mapping.h
    Mapping.cpp
    omnidome.cpp
    OutputPreview.h
    OutputPreview.cpp
    PluginList.h
    PluginList.cpp
    RecentSessions.h
    RecentSessions.cpp
    Scene.h
    Scene.cpp
    SceneGLView.h
    SceneGLView.cpp
    ScreenItem.h
    ScreenItem.cpp
    ScreenSetup.h
    ScreenSetup.cpp
    ToolBar.cpp
    ToolBar.h
    TuningGLView.h
    TuningGLView.cpp
    Warp.h
    Warp.cpp
    rc/omnidome.qrc
    )


IF(APPLE)
add_executable(omnidome MACOSX_BUNDLE ${SOURCES})

    SET(MACOSX_BUNDLE_INFO_STRING "${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_GUI_IDENTIFIER "com.${OMNI_VENDOR_SHORT}.${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${VERSION_STRING}")
    SET(MACOSX_BUNDLE_BUNDLE_NAME "${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_BUNDLE_PACKAGE_TYPE "APPL")
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_STRING}")
    SET(MACOSX_BUNDLE_ICON_FILE "${BUILD_TARGET}")
    SET(MACOSX_BUNDLE_COPYRIGHT "(C) ${OMNI_YEAR}. ${OMNI_URL}")
    set_target_properties(${BUILD_TARGET} PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/data/Info.plist")

    SET_SOURCE_FILES_PROPERTIES("${CMAKE_CURRENT_SOURCE_DIR}/data/omnidome.icns"
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
    )

    SET(APP_OUTPUT_DIR omnidome.app)

    # Make directory for icon and copy it to Contents/Resources
    add_custom_command(TARGET omnidome PRE_BUILD
                      COMMAND ${CMAKE_COMMAND} -E make_directory
                      ${APP_OUTPUT_DIR}/Contents/Resources
    )
    add_custom_command(TARGET omnidome POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       "${CMAKE_CURRENT_SOURCE_DIR}/data/omnidome.icns"
                       "${APP_OUTPUT_DIR}/Contents/Resources" )

                   add_custom_command(TARGET omnidome POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       "${CMAKE_CURRENT_SOURCE_DIR}/data/Info.plist"
                       "${APP_OUTPUT_DIR}/Contents" )

ADD_CUSTOM_TARGET(
      bundle
      ${Qt5_DIR}/../../../bin/macdeployqt ${CMAKE_CURRENT_BINARY_DIR}/${APP_OUTPUT_DIR} -dmg
        DEPENDS omnidome
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Make DMG file for MacOS X."
        VERBATIM
)
ELSE()

add_executable(omnidome ${SOURCES})
if (WIN32)
    target_sources(omnidome PRIVATE omnidome.rc)
endif()



ENDIF()

target_include_directories(omnidome
    PUBLIC
        ../lib/include
        ../widgetlib/include
    PRIVATE
        .
)

target_link_libraries(omnidome
    PUBLIC
        Qt5::Widgets
        omni
        omniwidget
)



