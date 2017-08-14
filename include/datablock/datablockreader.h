#ifndef DATABLOCK_H
#define DATABLOCK_H



#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>


#include "include/datablock/datablocktype.h"
#include "include/gameinfo/message.h"
#include "include/gameinfo/room.h"
#include "include/gameinfo/player.h"
#include "include/gameinfo/drawingtooltype.h"
#include "include/gameinfo/errorcode.h"



// Receive information from a client or a server through a TCP socket
class DataBlockReader : public QObject
{
	Q_OBJECT

	// Variables
	private :
		QTcpSocket* socket;
		QDataStream* socketStream;

		int nextSizeToRead;
		DataBlockType nextDataBlockType;

	// Constructor
	public :
		DataBlockReader(QTcpSocket* socket, QObject* parent = NULL);

	// Destructor
	public :
		~DataBlockReader();

	// Qt slots
	public slots :
		//
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



#endif // DATABLOCK_H
