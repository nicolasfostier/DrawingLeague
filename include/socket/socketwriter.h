#ifndef SOCKETWRITER_H
#define SOCKETWRITER_H



#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QBuffer>


#include "include/socket/blocktype.h"
#include "include/socket/blockwriter.h"
#include "include/gameinfo/message.h"
#include "include/gameinfo/room.h"
#include "include/gameinfo/player.h"
#include "include/gameinfo/drawingtooltype.h"
#include "include/gameinfo/errorcode.h"



// Send information to a client or a server through a TCP socket
class SocketWriter : public QObject
{
	Q_OBJECT

	// Variables
	private :
		QTcpSocket* socket;

	// Constructor
	public :
		SocketWriter(QTcpSocket* socket, QObject* parent = NULL);

	// Destructor
	public :
		~SocketWriter();

	// Qt slots
	public slots :
		void sendReadyToReceive();
		void sendEnterTheGame(Player player, QString gameVersion);
		void sendHasEnteredTheGame();
		void sendGameError(ErrorCode errorCode);

		void sendPlayerEntering(QString pseudo);
		void sendPlayerOnline(Player player);
		void sendPlayerLeaving(QString pseudo);

		void sendRoom(Room room);

		void sendGameStarting();
		void sendRoundStarting(quint32 round, QString artist, QString word, quint32 pointToWin);
		void sendRoundEnding(QString word);
		void sendGameEnding(QString winner);

		void sendChat(Message msg);
		void sendAnswer(Message msg);
		void sendAnswerFound(QString pseudo, quint32 pointWon);
		void sendAnswerClose();

		void sendHint(QString hint = QString());
		void sendSkipWord();

		void sendReady();

		void sendDrawingToolType(DrawingToolType drawingToolType);
		void sendDrawingToolColor(QColor color);
		void sendDrawingToolWidth(int width);

		void sendCanvasReset();

		void sendCanvasMousePressEvent(QPoint pos);
		void sendCanvasMouseMoveEvent(QPoint pos);
		void sendCanvasMouseReleaseEvent(QPoint pos);

		void sendReadyNeeded(int nbReadyNeeded);
};



#endif // SOCKETWRITER_H
