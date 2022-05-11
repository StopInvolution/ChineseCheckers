QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = logo.ico

SOURCES += \
    chessboard.cpp \
    initwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    marble.cpp \
    mul_initwidget.cpp \
    networkUtil.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    player.cpp \
    querydialog.cpp \
    room.cpp \
    serverwidget.cpp \
    util.cpp \
    waiting_room.cpp \
    widget.cpp

HEADERS += \
    chessboard.h \
    initwidget.h \
    mainwindow.h \
    marble.h \
    mul_initwidget.h \
    networkUtil.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    player.h \
    querydialog.h \
    room.h \
    serverwidget.h \
    settings.h \
    util.h \
    waiting_room.h \
    widget.h

FORMS += \
    initwidget.ui \
    mainwindow.ui \
    mul_initwidget.ui \
    querydialog.ui \
    serverwidget.ui \
    waiting_room.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
