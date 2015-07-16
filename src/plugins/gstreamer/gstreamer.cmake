# GStreamer Plugin Makefile

set(OMNI_GSTREAMER TRUE)

set(OMNI_GSTREAMER_PATH
  ${CMAKE_SOURCE_DIR}/src/plugins/gstreamer
)

include_directories( 
  /usr/include/glib-2.0 
  /usr/lib/x86_64-linux-gnu/glib-2.0/include 
  /usr/include/gstreamer-1.0
)

set(OMNI_GSTREAMER_LIBS
    gthread-2.0
    gio-2.0
    gobject-2.0
    glib-2.0
    gmodule-2.0
    gstreamer-1.0
    gstpbutils-1.0
    gstbase-1.0
    gstapp-1.0
)

set(OMNI_GSTREAMER_HEADER_FILES
  ${OMNI_GSTREAMER_PATH}/GLDisplay.h
)

set(OMNI_GSTREAMER_GUI_CPP_FILES
  ${OMNI_GSTREAMER_PATH}/GLDisplay.cpp
)

set(omni_gui_cpp_files ${omni_gui_cpp_files} ${OMNI_GSTREAMER_GUI_CPP_FILES} PARENT_SCOPE)
set(omni_headers ${omni_headers} ${OMNI_GSTREAMER_HEADER_FILES} PARENT_SCOPE)
set(omni_extra_libs ${omni_extra_libs} ${OMNI_GSTREAMER_LIBS} PARENT_SCOPE)


macro (omni_gstreamer_test)

  ADD_EXECUTABLE(plugin_test_GStreamer
  ${OMNI_GSTREAMER_PATH}/test_GStreamer.cpp 
  ${omni_src}  
  )

  TARGET_LINK_LIBRARIES(plugin_test_GStreamer 
    ${OMNI_GSTREAMER_LIBS}
    ${QT5_LIBRARIES} ${omni_extra_libs} 
    slim-widgetz 
    omni 
  )
endmacro()


