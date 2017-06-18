TARGET = DrawingLeagueDedicatedServer

win32{
    QMAKE_TARGET_PRODUCT = "Drawing League Dedicated Server"
    QMAKE_TARGET_DESCRIPTION = "Drawing League Dedicated Server"
    QMAKE_TARGET_COMPANY = "Nicolas Fostier"
    QMAKE_TARGET_COPYRIGHT = "Nicolas Fostier"
    RC_ICONS += res/images/logo.ico
}

macx{
    QMAKE_FRAMEWORK_BUNDLE_NAME = "Drawing League Dedicated Server"
    ICON = res/images/logo.icns
}

VERSION = 1.0.1



TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

QT +=   core \
        gui \
        network \
        widgets \

SOURCES +=  src/server/servermain.cpp \
            src/server/server.cpp \
            src/server/serverthread.cpp \
            src/gameinfo/message.cpp \
            src/gameinfo/player.cpp \
            src/gameinfo/room.cpp \
            src/gameinfo/drawingtooltype.cpp \
            src/datablock/datablockreader.cpp \
            src/datablock/datablocktype.cpp \
            src/datablock/datablockwriter.cpp \

HEADERS +=  include/server/server.h \
            include/server/serverthread.h \
            include/gameinfo/message.h \
            include/gameinfo/player.h \
            include/gameinfo/room.h \
            include/gameinfo/drawingtooltype.h \
            include/datablock/datablockreader.h \
            include/datablock/datablocktype.h \
            include/datablock/datablockwriter.h \

RESOURCES +=    res/dictionaries.qrc \
                res/lang.qrc

TRANSLATIONS += res/lang/drawingleaguededicatedserver_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
