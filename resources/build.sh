#!/bin/sh

output=$1

cd resources/
rm -f ${output}/Editor_Navigation.zip
zip -9 ${output}/Editor_Navigation.zip \
    manifest.xml *.xrc \
    images/editor_navigation/16x16/*.png \
    images/editor_navigation/20x20/*.png \
    images/editor_navigation/24x24/*.png \
    images/editor_navigation/28x28/*.png \
    images/editor_navigation/32x32/*.png \
    images/editor_navigation/40x40/*.png \
    images/editor_navigation/48x48/*.png \
    images/editor_navigation/56x56/*.png \
    images/editor_navigation/64x64/*.png
cd ../

