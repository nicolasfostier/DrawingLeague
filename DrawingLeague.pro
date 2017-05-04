QT  +=  core gui \
        widgets \
        network

SOURCES +=  src/main.cpp

HEADERS += 



TARGET = DrawingLeague
TEMPLATE = app
VERSION = 0.0.1
RC_ICONS += res/images/app.ico
QMAKE_TARGET_PRODUCT = "Drawing League"
QMAKE_TARGET_COMPANY = "Nicolas Fostier"
QMAKE_TARGET_COPYRIGHT = "Nicolas Fostier"
QMAKE_TARGET_DESCRIPTION = "Drawing League"
TRANSLATIONS = res/lang/drawingleague_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
