#!/bin/bash
# Shell script for running omnidome on linux
# Omnidome is located in /usr/share/Omnidome
appname=`basename $0 | sed s,\.sh$,,`
dirname=/usr/share/Omnidome 
LD_LIBRARY_PATH=${dirname}/lib
QT_QPA_PLATFORM_PLUGIN_PATH=${dirname}/plugins/platforms
QT_XCB_GL_INTEGRATION=xcb_glx
QT_PLUGIN_PATH=${dirname}/plugins
export LD_LIBRARY_PATH QT_PLUGIN_PATH QT_QPA_PLATFORM_PLUGIN_PATH QT_XCB_GL_INTEGRATION
$dirname/bin/$appname "$@"

