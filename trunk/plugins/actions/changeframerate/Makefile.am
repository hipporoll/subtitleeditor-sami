plugin_name = changeframerate
pluginlibdir = $(PACKAGE_PLUGIN_LIB_DIR)/actions
plugindescriptiondir = $(PACKAGE_PLUGIN_DESCRIPTION_DIR)/actions
actionplugindevdir = $(abs_srcdir)
uidir = $(PACKAGE_PLUGIN_SHARE_DIR)/$(plugin_name)

INCLUDES = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	$(SUBTITLEEDITOR_CFLAGS) \
	-DSE_PLUGIN_PATH_DEV=\"$(actionplugindevdir)\" \
	-DSE_PLUGIN_PATH_UI=\"$(uidir)\"

pluginlib_LTLIBRARIES = \
	libchangeframerate.la

libchangeframerate_la_SOURCES = \
	changeframerate.cc

libchangeframerate_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libchangeframerate_la_LIBADD = $(SUBTITLEEDITOR_LIBS) -L$(top_srcdir)/src -lsubtitleeditor

plugindescription_in_files = changeframerate.se-plugin.in
plugindescription_DATA = $(plugindescription_in_files:.se-plugin.in=.se-plugin)

@INTLTOOL_SE_PLUGIN_RULE@

ui_DATA = dialog-change-framerate.ui

EXTRA_DIST = $(plugindescription_in_files) $(ui_DATA)

CLEANFILES = $(plugindescription_DATA) Makefile.am~ *.cc~ *.h~ *.ui~ *.uip *.uip.bak *.in~
