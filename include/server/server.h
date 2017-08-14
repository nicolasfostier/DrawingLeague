#ifndef SERVER_H
#define SERVER_H



#include <algorithm>
#include <random>
#include <time.h>


#include <QTcpServer>
#include <QThread>
#include <QColor>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QApplication>


#include "include/server/serverthread.h"
#include "include/gameinfo/room.h"
#include "include/gameinfo/drawingtooltype.h"
#include "include/gameinfo/errorcode.h"
#include "include/server/wordtools.h"



// Host a room
class Server : public QTcpServer
{
	Q_OBJECT

	// Variables
	private :
		int port;
		QHash<QString, ServerThread*> players;

		Room room;

		QList<ServerThread*> artistsQueue;
		ServerThread* artist;

		QString dictionaryPath;
		QList<QString> words;
		QList<QString> wordsQueue;
		QString word;

		int playerFoundAnswer;
		bool oneHasFound;
		int hintGiven;

		DrawingToolType drawingToolType;
		QColor drawingToolColor;
		int drawingToolWidth;

		QTimer* timerRound;
		QTimer* timerBetweenRound;


	// Constructor
	public:
		Server(int port, Room room, QString dictionaryPath);

	// Destructor
	public :
		~Server();

	// Methods
	public :
		void loadDictionary(QString dictionaryPath);

		int playerReady();
		int nbReadyNeeded();

		void nextArtist();
		void nextWord();

	// Qt slots
	public slots :
		void launch();

		void handleNewConnection();
			void verifyNewPlayer();
			void setupNewPlayer();
		void removePlayer(QString pseudo, ServerThread* player, bool hasFound);

		void processAnswer(Message msg);
		void processChatCommand(Message msg);

		void hint();
		void skipWord();

		void setPlayerReady();

		void changeDrawingToolType(DrawingToolType drawingToolType);
		void changeDrawingToolColor(QColor color);
		void changeDrawingToolWidth(int width);

		void canvasReset();

		void canvasMousePressEvent(QPoint pos);
		void canvasMouseMoveEvent(QPoint pos);
		void canvasMouseReleaseEvent(QPoint pos);

		void startGame();
		void startRound();
		void endRound();
		void endGame();

		void sendReadyNeeded();

	// Signals
	signals :
		void isReady();
};



#endif // SERVER_H
