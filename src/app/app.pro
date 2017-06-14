TEMPLATE = app

TARGET = omnidome

include(../common.pri)

LIBS += ../lib/libomni.a

PRE_TARGETDEPS += ../lib/libomni.a

HEADERS += \
  About.h \
  AboutGL.h \
  Application.h \
  Blend.h \
  BrushPreview.h \
  Canvas.h \
  ColorCorrection.h \
  DockWidget.h \
  Export.h \
  Input.h \
  MainWindow.h \
  Mapping.h \
  OutputPreview.h \
  PluginList.h \
  RecentSessions.h \
  Scene.h \
  ScreenItem.h \
  ScreenSetup.h \
  ToolBar.h \
  Warp.h \
  proj/TitleBar.h \
  proj/Tuning.h \
  proj/TuningLayout.h \
  proj/TuningList.h

SOURCES += \
  About.cpp \
  AboutGL.cpp \
  Application.cpp \
  Blend.cpp \
  BrushPreview.cpp \
  Canvas.cpp \
  ColorCorrection.cpp \
  DockWidget.cpp \
  Export.cpp \
  Input.cpp \
  MainWindow.cpp \
  Mapping.cpp \
  OutputPreview.cpp \
  PluginList.cpp \
  RecentSessions.cpp \
  Scene.cpp \
  ScreenItem.cpp \
  ScreenSetup.cpp \
  ToolBar.cpp \
  Warp.cpp \
  proj/TitleBar.cpp \
  proj/Tuning.cpp \
  proj/TuningLayout.cpp \
  proj/TuningList.cpp \
  omnidome.cpp

FORMS += \
  ui/omni_ui_About.ui \
  ui/omni_ui_Blend.ui \
  ui/omni_ui_BlendMaskSettings.ui \
  ui/omni_ui_Canvas.ui \
  ui/omni_ui_ColorCorrection.ui \
  ui/omni_ui_Export.ui \
  ui/omni_ui_Input.ui \
  ui/omni_ui_Live.ui \
  ui/omni_ui_MainWindow.ui \
  ui/omni_ui_Mapping.ui \
  ui/omni_ui_MappingWidget.ui \
  ui/omni_ui_ProjectorWidget.ui \
  ui/omni_ui_Scene.ui \
  ui/omni_ui_ScreenSettings.ui \
  ui/omni_ui_Warp.ui \
  ui/omni_ui_WarpGrid.ui \
  ui/omni_ui_WarpGridSettings.ui \
  ui/omni_ui_WarpSettings.ui

RESOURCES += rc/omnidome.qrc
