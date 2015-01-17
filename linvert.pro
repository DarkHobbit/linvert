# -------------------------------------------------
# Project created by QtCreator 2015-01-03T00:25:16
# -------------------------------------------------
# TODO compat with QT5
QT += core \
    xml
QT -= gui
TARGET = linvert
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    linvertapplication.cpp \
    lisourcefile.cpp \
    liuifile.cpp \
    lisourcelist.cpp \
    licppfile.cpp
HEADERS += linvertapplication.h \
    lisourcefile.h \
    liuifile.h \
    lisourcelist.h \
    licppfile.h
