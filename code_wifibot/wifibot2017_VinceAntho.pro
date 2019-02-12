#-------------------------------------------------
#
# Project created by QtCreator 2017-06-15T16:25:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network
QT += webenginewidgets

TARGET = wifibot2017_VinceAntho
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sensors.cpp \
    tcpconnection.cpp

HEADERS  += mainwindow.h \
    sensors.h \
    tcpconnection.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressources.qrc
