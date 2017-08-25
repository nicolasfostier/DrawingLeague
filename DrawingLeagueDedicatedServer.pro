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

VERSION = 1.1.1



TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

QT +=   core \
		gui \
		network \
		widgets \

SOURCES +=	src/gameinfo/drawingtooltype.cpp \
			src/gameinfo/errorcode.cpp \
			src/gameinfo/message.cpp \
			src/gameinfo/player.cpp \
			src/gameinfo/playerscore.cpp \
			src/gameinfo/room.cpp \
			src/loghandler.cpp \
			src/server/server.cpp \
			src/server/servermain.cpp \
			src/server/serverthread.cpp \
			src/server/wordtools.cpp \
			src/socket/blockreader.cpp \
			src/socket/blockwriter.cpp \
			src/socket/blocktype.cpp \
			src/socket/connection.cpp \
			src/socket/socketreader.cpp \
			src/socket/socketwriter.cpp \

HEADERS +=  include/gameinfo/drawingtooltype.h \
			include/gameinfo/errorcode.h \
			include/gameinfo/message.h \
			include/gameinfo/player.h \
			include/gameinfo/playerscore.h \
			include/gameinfo/room.h \
			include/loghandler.h \
			include/server/server.h \
			include/server/serverthread.h \
			include/server/wordtools.h \
			include/socket/blockreader.h \
			include/socket/blockwriter.h \
			include/socket/blocktype.h \
			include/socket/connection.h \
			include/socket/socketreader.h \
			include/socket/socketwriter.h \

RESOURCES +=    res/dictionaries.qrc \
				res/lang.qrc

TRANSLATIONS += res/lang/drawingleaguededicatedserver_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
