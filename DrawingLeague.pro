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

VERSION = 1.1



TEMPLATE = app

QT  +=  core \
		gui \
		multimedia \
		network \
		widgets \

SOURCES +=  src/main.cpp \
			src/datablock/datablockreader.cpp \
			src/datablock/datablocktype.cpp \
			src/datablock/datablockwriter.cpp \
			src/gameinfo/connection.cpp \
			src/gameinfo/drawingtooltype.cpp \
			src/gameinfo/errorcode.cpp \
			src/gameinfo/message.cpp \
			src/gameinfo/player.cpp \
			src/gameinfo/playerscore.cpp \
			src/gameinfo/room.cpp \
			src/graphicsitem/canvas.cpp \
			src/graphicsitem/readybutton.cpp \
			src/loghandler.cpp \
			src/server/server.cpp \
			src/server/serverthread.cpp \
			src/server/wordtools.cpp \
			src/shapes/polyline.cpp \
			src/shapes/shape.cpp \
			src/updatechecker.cpp \
			src/widgets/canvasview.cpp \
			src/widgets/roominfo.cpp \
			src/widgets/roominfopart.cpp \
			src/widgets/timer.cpp \
			src/windows/createroomwindow.cpp \
			src/windows/joinroomwindow.cpp \
			src/windows/mainwindow.cpp \

HEADERS +=  include/datablock/datablockreader.h \
			include/datablock/datablocktype.h \
			include/datablock/datablockwriter.h \
			include/gameinfo/connection.h \
			include/gameinfo/drawingtooltype.h \
			include/gameinfo/errorcode.h \
			include/gameinfo/message.h \
			include/gameinfo/player.h \
			include/gameinfo/playerscore.h \
			include/gameinfo/room.h \
			include/graphicsitem/canvas.h \
			include/graphicsitem/readybutton.h \
			include/loghandler.h \
			include/server/server.h \
			include/server/serverthread.h \
			include/server/wordtools.h \
			include/shapes/polyline.h \
			include/shapes/shape.h \
			include/updatechecker.h \
			include/widgets/canvasview.h \
			include/widgets/roominfo.h \
			include/widgets/roominfopart.h \
			include/widgets/timer.h \
			include/windows/createroomwindow.h \
			include/windows/joinroomwindow.h \
			include/windows/mainwindow.h \

RESOURCES +=    res/dictionaries.qrc \
				res/images.qrc \
				res/lang.qrc \
				res/sound.qrc \

TRANSLATIONS += res/lang/drawingleague_fr.ts



DEFINES += QT_DEPRECATED_WARNINGS
