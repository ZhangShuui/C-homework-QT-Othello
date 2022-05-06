#-------------------------------------------------
#
# Project created by QtCreator 2022-04-11T19:47:37
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Othello
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
    dialog.cpp \
    mainboardwindow.cpp \
    mythread.cpp \
    mcts_pyqt.cpp \
    timethread.cpp \
    aithread.cpp \
    soundthread.cpp

HEADERS += \
        mainwindow.h \
    dialog.h \
    mainboardwindow.h \
    mythread.h \
    mcts_pyqt.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    mainboardwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    background.jpeg \
    board.py \
    main.py \
    pic/black.png \
    pic/music.wav

RESOURCES += \
    res.qrc

INCLUDEPATH += C:/Users/zsr/AppData/Local/Programs/Python/Python37/include

LIBS += -LC:/Users/zsr/AppData/Local/Programs/Python/Python37/libs -lpython37
