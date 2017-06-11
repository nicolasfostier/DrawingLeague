TARGET = DrawingLeague

win32{
    QMAKE_TARGET_PRODUCT = "Drawing League"
    QMAKE_TARGET_DESCRIPTION = "Drawing League"
    QMAKE_TARGET_COMPANY = "Nicolas Fostier"
    QMAKE_TARGET_COPYRIGHT = "Nicolas Fostier"
    RC_ICONS += res/images/logo.ico
}

macx{
    QMAKE_FRAMEWORK_BUNDLE_NAME = "Drawing League"
    ICON = res/images/logo.icns
}

TEMPLATE = app

VERSION = 1.0



QT  +=  core gui \
        widgets \
        network \
        multimedia

SOURCES +=  src/main.cpp \
            src/windows/mainwindow.cpp \
            src/checkforupdate.cpp \
            src/widgets/canvas.cpp \
            src/shapes/polyline.cpp \
            src/shapes/shape.cpp \
            src/gameinfo/room.cpp \
            src/windows/joinroomwindow.cpp \
            src/windows/createroomwindow.cpp \
            src/server/server.cpp \
            src/server/serverthread.cpp \
            src/datablock/datablocktype.cpp \
            src/datablock/datablockreader.cpp \
            src/datablock/datablockwriter.cpp \
            src/gameinfo/message.cpp \
            src/widgets/roominfo.cpp \
            src/widgets/roominfopart.cpp \
            src/gameinfo/player.cpp \
            src/drawingtooltype.cpp

HEADERS +=  include/windows/mainwindow.h \
            include/checkforupdate.h \
            include/widgets/canvas.h \
            include/shapes/polyline.h \
            include/shapes/shape.h \
            include/gameinfo/room.h \
            include/windows/joinroomwindow.h \
            include/windows/createroomwindow.h \
            include/server/server.h \
            include/server/serverthread.h \
            include/datablock/datablocktype.h \
            include/datablock/datablockreader.h \
            include/datablock/datablockwriter.h \
            include/gameinfo/message.h \
            include/widgets/roominfo.h \
            include/widgets/roominfopart.h \
            include/gameinfo/player.h \
            include/drawingtooltype.h

RESOURCES +=    res/lang.qrc \
                res/images.qrc \
                res/dictionaries.qrc \
                res/sound.qrc

TRANSLATIONS = res/lang/drawingleague_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
