#!/bin/sh

linuxdeploy --appdir ./appdir -e ../build/bin/QAddApp -d ./QAddApp.desktop -i ./qaddapp_logo.png --custom-apprun=./AppRun

ARCH=x86_64 appimagetool ./appdir/
