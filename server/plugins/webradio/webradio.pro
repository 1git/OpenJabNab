######################################################################
# Automatically generated by qmake (2.01a) sam. janv. 19 19:10:01 2008
######################################################################

TEMPLATE = lib
CONFIG -= #DEBUG
CONFIG += plugin qt debug
QT += network
QT -= gui
INCLUDEPATH += . ../../server ../../lib
TARGET = plugin_webradio
DESTDIR = ../../bin/plugins
DEPENDPATH += . ../../server ../../lib
LIBS += -L../../bin/ -lcommon
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj
win32 {
	QMAKE_CXXFLAGS_WARN_ON += -WX
}
unix {
	QMAKE_LFLAGS += -Wl,-rpath,\'\$$ORIGIN\'
	QMAKE_CXXFLAGS += -Werror
}

# Input
HEADERS += plugin_webradio.h
SOURCES += plugin_webradio.cpp
