#ifndef SOCKETREADER_H
#define SOCKETREADER_H



#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>


#include "include/socket/blocktype.h"
#include "include/socket/blockreader.h"
#include "include/gameinfo/message.h"
#include "include/gameinfo/room.h"
#include "include/gameinfo/player.h"
#include "include/gameinfo/drawingtooltype.h"
#include "include/gameinfo/errorcode.h"



// Receive information from a client or a server through a TCP socket
class SocketReader : public QObject
{
	Q_OBJECT

	// Variables
	private :
		QTcpSocket* socket;

		BlockType newBlockType;

	// Constructor
	public :
		SocketReader(QTcpSocket* socket, QObject* parent = NULL);

	// Destructor
	public :
		~SocketReader();

	// Qt slots
	public slots :
		void read();

	// Signals
	signals :
		void readyToReceive();
		void enterTheGameReceived(Player player, QString gameVersion);
		void hasEnteredTheGame();
		void gameErrorReceived(ErrorCode);

		void playerEnteringReceived(QString pseudo);
		void playerOnlineReceived(Player pseudo);
		void playerLeavingReceived(QString pseudo);

		void roomReceived(Room room);

		void gameStartingReceived();
		void roundStartingReceived(int round, QString artist, QString word, int pointToWin);
		void roundEndingReceived(QString word);
		void gameEndingReceived(QString winner);

		void chatReceived(Message msg);
		void answerReceived(Message msg);
		void answerFoundReceived(QString pseudo, int pointWon);
		void answerCloseReceived();

		void hintReceived(QString hint);
		void skipWordReceived();

		void readyReceived();

		void drawingToolTypeReceived(DrawingToolType drawingToolType);
		void drawingToolColorReceived(QColor color);
		void drawingToolWidthReceived(int width);

		void canvasResetReceived();

		void canvasMousePressEventReceived(QPoint pos);
		void canvasMouseMoveEventReceived(QPoint pos);
		void canvasMouseReleaseEventReceived(QPoint pos);

		void readyNeededReceived(int nbReadyNeeded);
};



#endif // SOCKETREADER_H
