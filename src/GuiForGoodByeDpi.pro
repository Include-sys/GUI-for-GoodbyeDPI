#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T22:06:28
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuiForGoodByeDpi
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


#Adming Rights and Setting ICON
win32-msvc* {
    RC_ICONS = images/icon.ico
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}

win32-g++ {
    RC_FILE = guiforgoodbyedpi.rc
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    settings.cpp \
    about.cpp

HEADERS += \
        mainwindow.h \
    settings.h \
    about.h

FORMS += \
        mainwindow.ui \
    settings.ui \
    about.ui

RESOURCES += \
    res.qrc
