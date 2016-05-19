Omnidome
========

Fulldome Projection Calibration Software Framework


1 How to build Omnidome
=======================

1.1 Prerequisites
=================

Omnidome uses the Qt Framework and C++11 standard library and language standard.
Its has currently been successfully build on MacOSX (Mavericks and Yosemite) and Ubuntu 14.04.

For both MacOSX and Ubuntu, install Qt 5.4 via online installer from the website http://www.qt.io.
Qt should be installed in the folder which lies on the same level as omnidome, e.g.:

/your-project-folder
 -/Qt
 -/omnidome

With cmake it is also possible to specify a manual path for Qt via commandline.
When running cmake with arguments:

cmake . -DQT_PATH=/your/path/to/qt

CMake is looking for Qt in this directory.
DO NOT use the ~ symbol when referring to your home folder, you will need to fully write down the folder name,
like /home/cr8tr instead.

1.2 Building on Ubuntu 14.04
============================

$ sudo apt-get install cmake git g++-4.9

in commandline.
You will also need cmake, which you can install with

$ sudo apt-get install cmake

Go in to omnidome folder, and run

./cmake-files/deploy_ubuntu.sh

You can start omnidome with
./bin/Release/omnidome


1.3 Building on MacOSX
======================

For a MacOSX build, you will need the XCode developer tools.
You also need homebrew (http://brew.sh) to install cmake, with

brew install cmake

From the terminal, go in to omnidome folder, and run

$ cmake . -DQT_PATH=/your/path/to/qt

You might to specify the Qt Path manually by passing the additional argument
-DQT_PATH=/your/path/to/qt to the cmake command.

DO NOT use the ~ symbol when referring to your home folder, you will need to fully write down the folder name,
like /Users/cr8tr instead.

If this error occurs while running CMake:
CMake Error at /Users/wilstonoreo/Projects/Qt/5.5/clang_64/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake:9 (message):
  Failed to find "gl.h" in

Please install XCode Developer Tools with "xcode-select --install".

Start compiling and make the dmg file with:

$ ./cmake-files/deploy_osx.sh

You can start omnidome with
./bin/Release/omnidome.app/Contents/MacOS/omnidome

The app bundle "omnidome.app" is in ./bin/Release


2 Release Notes
===============

Omnidome was only successfully compiled on MacOSX and Ubuntu 14.04.
