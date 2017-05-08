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
    src/checkforupdate.cpp

HEADERS +=  include/windows/mainwindow.h \
    include/checkforupdate.h

RESOURCES +=    res/lang.qrc \
                res/images.qrc

TRANSLATIONS = res/lang/drawingleague_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
