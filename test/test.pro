#-------------------------------------------------
#
# Project created by QtCreator 2014-08-19T01:29:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++11
MOC_DIR = moc
OBJECTS_DIR = obj

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/syncrequest/release/ -lsyncrequest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/syncrequest/debug/ -lsyncrequest
else:unix: LIBS += -L$$OUT_PWD/../lib/syncrequest/ -lsyncrequest

INCLUDEPATH += $$PWD/../lib/syncrequest
DEPENDPATH += $$PWD/../lib/syncrequest

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/arangodb/release/ -larangodb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/arangodb/debug/ -larangodb
else:unix: LIBS += -L$$OUT_PWD/../lib/arangodb/ -larangodb

INCLUDEPATH += $$PWD/../lib/arangodb
DEPENDPATH += $$PWD/../lib/arangodb
