#-------------------------------------------------
#
# Project created by QtCreator 2022-12-02T19:45:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        user.cpp\
        session.cpp

HEADERS  += mainwindow.h\
         user.h\
         session.h

FORMS    += mainwindow.ui

RESOURCES += \
    qtresources.qrc\
    user.json
