#!/bin/sh

output=$1

cd resources/
rm -f ${output}/Editor_Navigation.zip
zip -9 ${output}/Editor_Navigation.zip \
    manifest.xml *.xrc \
    images/editor_navigation/16x16/*.png \
    images/editor_navigation/22x22/*.png \
    images/editor_navigation/32x32/*.png
cd ../

