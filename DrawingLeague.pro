TARGET = DrawingLeague

win32{
    QMAKE_TARGET_PRODUCT = "Drawing League"
    QMAKE_TARGET_DESCRIPTION = "Drawing League"
    QMAKE_TARGET_COMPANY = "Nicolas Fostier"
    QMAKE_TARGET_COPYRIGHT = "Nicolas Fostier"
    #RC_ICONS += res/images/app.ico
}

macx{
    QMAKE_FRAMEWORK_BUNDLE_NAME = "Drawing League"
    #ICON = res/images/app.icns
}

TEMPLATE = app

VERSION = 0.0.1



QT  +=  core gui \
        widgets \
        network

SOURCES +=  src/main.cpp \
            src/windows/mainwindow.cpp \
            src/checkforupdate.cpp \
            src/widgets/canvas.cpp \
            src/shapes/polyline.cpp \
            src/shapes/shape.cpp \
    src/room.cpp \
    src/server.cpp \
    src/windows/joinroomwindow.cpp \
    src/windows/createroomwindow.cpp

HEADERS +=  include/windows/mainwindow.h \
            include/checkforupdate.h \
            include/widgets/canvas.h \
            include/shapes/polyline.h \
            include/shapes/shape.h \
    include/drawtooltype.h \
    include/room.h \
    include/server.h \
    include/windows/joinroomwindow.h \
    include/windows/createroomwindow.h \
    include/datasenttype.h

RESOURCES +=    res/lang.qrc \
                res/images.qrc

TRANSLATIONS = res/lang/drawingleague_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
