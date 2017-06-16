
CONFIG += c++11
CONFIG += object_parallel_to_source
CONFIG += ordered

QT += widgets

DEFINES += "OMNICALIBRATION_VERSION_STRING=\\\"1.0.0\\\""
DEFINES += "OMNIDOME_VERSION_STRING=\\\"1.0.1\\\""
DEFINES += OMNIC_USE_QT_GL

OMNI_INCLUDE = $$PWD/../include/omni

INCLUDEPATH += \
  $$PWD/../omnicalib/include \
  $$PWD/../include

# OBJECTIVE_SOURCES += foo.m
# ObjectiveC
