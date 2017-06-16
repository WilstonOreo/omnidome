
TEMPLATE = app

QT += testlib

CONFIG += testcase

include(../../common.pri)

CONFIG   -= app_bundle

SOURCES += tst_Serialization.cpp \

HEADERS += \
    TestObject.h \
