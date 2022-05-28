QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = logo.ico

SOURCES += \
    agent.cpp \
    chessboard.cpp \
    clickableqlabel.cpp \
    clientwidget.cpp \
    initwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    marble.cpp \
    mul_initwidget.cpp \
    mvector.cpp \
    networkUtil.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    player.cpp \
    room.cpp \
    serverplayer.cpp \
    serverwidget.cpp \
    settingswidget.cpp \
    settings.cpp \
    util.cpp \
    waiting_room.cpp \
    widget.cpp \
    windowAddress.cpp

HEADERS += \
    agent.h \
    chessboard.h \
    clickableqlabel.h \
    clientwidget.h \
    initwidget.h \
    mainwindow.h \
    marble.h \
    mul_initwidget.h \
    mvector.h \
    networkUtil.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    player.h \
    room.h \
    serverplayer.h \
    serverwidget.h \
    settings.h \
    settingswidget.h \
    util.h \
    waiting_room.h \
    widget.h \
    windowAddress.h

FORMS += \
    clientwidget.ui \
    initwidget.ui \
    mainwindow.ui \
    mul_initwidget.ui \
    serverwidget.ui \
    settingswidget.ui \
    waiting_room.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
