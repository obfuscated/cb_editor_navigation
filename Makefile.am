
################ build ########################

INCLUDES = $(WX_CXXFLAGS)

cb_plugin_lib_LTLIBRARIES = libEditor_Navigation.la
libEditor_Navigation_la_SOURCES = src/editor_navigation.cpp src/list_dialog.cpp

noinst_HEADERS = src/list_dialog.h src/common.h src/editor_navigation.h

libEditor_Navigation_la_LDFLAGS = -avoid-version -shared -no-undefined


################ resources ########################

cb_plugin_data_DATA = Editor_Navigation.zip

Editor_Navigation_RESOURCES = resources/build.sh resources/editor_navigation_toolbar_16x16.xrc resources/editor_navigation_toolbar_22x22.xrc  resources/manifest.xml \
				 resources/images/editor_navigation/16x16/forward.png  resources/images/editor_navigation/16x16/backward.png \
				 resources/images/editor_navigation/22x22/forward.png  resources/images/editor_navigation/22x22/backward.png 

EXTRA_DIST = $(Editor_Navigation_RESOURCES) editor_navigation.cbp
CLEANFILES = Editor_Navigation.zip

Editor_Navigation.zip: $(Editor_Navigation_RESOURCES)
	PWD=`pwd` cd $(srcdir)/resources/ && zip -9 $(PWD)/Editor_Navigation.zip manifest.xml *.xrc images/editor_navigation/16x16/*.png images/editor_navigation/22x22/*.png > /dev/null

