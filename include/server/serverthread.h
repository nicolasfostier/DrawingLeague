#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H



#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>


#include "include/socket/socketreader.h"
#include "include/socket/socketwriter.h"
#include "include/socket/connection.h"
#include "include/gameinfo/room.h"
#include "include/gameinfo/message.h"
#include "include/gameinfo/player.h"



// Handle a connection to a player in its own thread
class ServerThread : public QObject
{
	Q_OBJECT

	// Variables
	private :
		qintptr socketDescriptor;
		Connection* connection;

		Player* player;
		QString gameVersion;

		QTimer* timeout;

	// Getter
	public :
		QTcpSocket* getSocket();
		Player* getPlayer();
		QString getGameVersion();
		SocketReader* getSocketReader();
		SocketWriter* getSocketWriter();

	// Constructeur
	public :
		ServerThread(qintptr socketDescriptor);

	// Destructor
	public :
		~ServerThread();

	// Qt slots
	public slots :
		void launch();
		void setupPlayerInfo(Player player, QString gameVersion);
		void hasEnteredTheGame();
		void cantEnterTheGame(ErrorCode errorCode);

	// Qt signals
	signals :
		void wantToEnterTheGame();
		void readyToReceive();
		void playerLeaving(QString pseudo, ServerThread* serverThread, bool hasFound);

	// Operators
	public :
		bool operator!=(ServerThread serverThread);
};



#endif // SERVERTHREAD_H
