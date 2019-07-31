#-------------------------------------------------
#
# Project created by QtCreator 2019-04-28T19:55:49
#
#-------------------------------------------------
QT += core network xml

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UtgardLauncher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    md5.cpp \
    registration.cpp \
    playerwindow.cpp \
    d_delete.cpp \
    d_change.cpp \
    d_player.cpp \
    d_author.cpp \
    d_wiki.cpp \
    d_donate.cpp

HEADERS += \
        mainwindow.h \
    md5.h \
    registration.h \
    playerwindow.h \
    d_delete.h \
    d_change.h \
    d_player.h \
    d_author.h \
    d_wiki.h \
    d_donate.h

FORMS += \
        mainwindow.ui \
    registration.ui \
    playerwindow.ui \
    d_delete.ui \
    d_change.ui \
    d_player.ui \
    d_author.ui \
    d_wiki.ui \
    d_donate.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc


DISTFILES += \
    icon.rc
