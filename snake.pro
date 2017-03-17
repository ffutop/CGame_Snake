#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T11:18:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app


SOURCES += main.cpp\
    block.cpp \
    snake.cpp \
    gameController.cpp

HEADERS  += \
    block.h \
    snake.h \
    gameController.h

FORMS    += \
    gameController.ui

RESOURCES += \
    icon.qrc
