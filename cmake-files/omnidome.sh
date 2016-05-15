#!/bin/bash
# Shell script for running omnidome in /usr/share/omnidome

appname=`basename $0 | sed s,\.sh$,,`
dirname=/usr/share/omnidome 

LD_LIBRARY_PATH=${dirname}/libs
export LD_LIBRARY_PATH
$dirname/$appname "$@"
