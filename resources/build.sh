#!/bin/sh

cd resources/
rm ../Editor_Navigation.zip
zip -9 ../Editor_Navigation.zip manifest.xml *.xrc images/editor_navigation/16x16/*.png images/editor_navigation/22x22/*.png
cd ../

