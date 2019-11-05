Omnidome
========

Fulldome Projection Calibration Software Framework

How to build Omnidome
=======================

Omnidome uses the Qt Framework (minimum Version 5.9) and C++11 standard library and language standard.
Its has currently been successfully build on MacOSX (Mavericks and Yosemite).

For both MacOSX and Ubuntu, install Qt 5.9 via online installer from the website http://www.qt.io.
Qt is supposed to be installed in the home folder.
You can compile Qt with Qt Creator and selecting CMakeLists.txt as project.

For a MacOSX build, you will need the XCode developer tools.
Please install XCode Developer Tools with "xcode-select --install".
You also need homebrew (http://brew.sh) to install cmake, with

$ brew install cmake

On Ubuntu, install latest cmake with:

$ sudo purge cmake ; sudo snap install cmake --classic

With cmake it is also possible to specify a manual path for Qt via commandline.
When running cmake with arguments:

$ mkdir ../omnidome_build ; cd ../omnidome_build
$ CMAKE_PREFIX_PATH=/path/to/Qt/5.9.8/clang_64/lib/cmake cmake ../omnidome

Compile omnidome with

$ make

Start compiling and make the dmg file with:

$ make bundle


