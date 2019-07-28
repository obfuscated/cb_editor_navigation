
################ build ########################
ACLOCAL_AMFLAGS = --install -I m4

AM_CPPFLAGS = $(WX_CXXFLAGS)

cb_plugin_lib_LTLIBRARIES = libEditor_Navigation.la
libEditor_Navigation_la_SOURCES = src/editor_navigation.cpp src/list_dialog.cpp

noinst_HEADERS = src/list_dialog.h src/common.h src/editor_navigation.h

libEditor_Navigation_la_LDFLAGS = -avoid-version -shared -no-undefined


################ resources ########################

cb_plugin_data_DATA = Editor_Navigation.zip

Editor_Navigation_RESOURCES = \
	resources/editor_navigation_toolbar.xrc \
	resources/manifest.xml \
	resources/images/editor_navigation/16x16/backward.png \
	resources/images/editor_navigation/16x16/forward.png \
	resources/images/editor_navigation/20x20/backward.png \
	resources/images/editor_navigation/20x20/forward.png \
	resources/images/editor_navigation/22x22/backward.png \
	resources/images/editor_navigation/22x22/forward.png \
	resources/images/editor_navigation/24x24/backward.png \
	resources/images/editor_navigation/24x24/forward.png \
	resources/images/editor_navigation/28x28/backward.png \
	resources/images/editor_navigation/28x28/forward.png \
	resources/images/editor_navigation/32x32/backward.png \
	resources/images/editor_navigation/32x32/forward.png \
	resources/images/editor_navigation/40x40/backward.png \
	resources/images/editor_navigation/40x40/forward.png \
	resources/images/editor_navigation/48x48/backward.png \
	resources/images/editor_navigation/48x48/forward.png \
	resources/images/editor_navigation/56x56/backward.png \
	resources/images/editor_navigation/56x56/forward.png \
	resources/images/editor_navigation/64x64/backward.png \
	resources/images/editor_navigation/64x64/forward.png

EXTRA_DIST = \
	$(Editor_Navigation_RESOURCES) \
	editor_navigation.cbp \
	resources/build.sh \
	resources/create_atlas.py \
	resources/svg/backward_16.svg \
	resources/svg/backward_20.svg \
	resources/svg/backward_24.svg \
	resources/svg/backward_28.svg \
	resources/svg/backward_32.svg \
	resources/svg/forward_16.svg \
	resources/svg/forward_20.svg \
	resources/svg/forward_24.svg \
	resources/svg/forward_28.svg \
	resources/svg/forward_32.svg

CLEANFILES = Editor_Navigation.zip

Editor_Navigation.zip: $(Editor_Navigation_RESOURCES)
	PWD=`pwd` \
		cd $(srcdir)/resources/ && \
		zip -9 $(PWD)/Editor_Navigation.zip $(Editor_Navigation_RESOURCES:resources/%=%) > /dev/null
