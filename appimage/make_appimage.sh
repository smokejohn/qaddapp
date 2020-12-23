#!/bin/sh

# needs linuxdeploy and linuxdeploy-plugin-qt
# needs appimagetool

# Make sure to delete the appdir if building fails, subsequent execution of linuxdeploy will reuse existing configs and data otherwise

linuxdeploy --appdir ./appdir -e ../build/bin/qaddapp -d ./qaddapp.desktop -i ./qaddapp_logo_256.png --custom-apprun=./AppRun -p qt

ARCH=x86_64 appimagetool ./appdir/
