#ifndef ROOM_H
#define ROOM_H



#include <QString>
#include <QTimer>
#include <QDataStream>
#include <QTranslator>

#include <QDateTime>
#include <QDebug>



// Handle informations about a game room
class Room : public QObject
{
	Q_OBJECT

	// Variables
	private :
		QString roomName;
		int numberOfRounds;
		int maxPlayers;
		int timeByRound;
		int timeAfterFirstGoodAnswer;

		int round;
		QString artist;
		QString word;
		int pointToWin;
		int timeRemainingMs;

	// Getter
	public :
		QString getRoomName();
		int getNumberOfRounds();
		int getMaxPlayers();
		int getTimeByRound();
		int getTimeAfterFirstGoodAnswer();

		int getCurrentRound();
		QString getArtist();
		QString getWord();
		int getPointToWin();
		int getTimeRemainingMs();

	// Setter
	public :
		void setRoomName(QString roomName);
		void setNumberOfRounds(int numberOfRounds);
		void setMaxPlayers(int maxPlayers);
		void setTimeByRound(int timeByRound);
		void setTimeAfterFirstGoodAnswer(int timeAfterFirstGoodAnswer);

		void setCurrentRound(int round);
		void setArtist(QString artist);
		void setWord(QString word);
		void setPointToWin(int pointToWin);
		void setTimeRemainingMs(int timeRemainingMs);

	// Constructeur
	public :
		Room(QString roomName = tr("Offline"), int numberOfRounds = 10, int maxPlayers = 10, int timeByRound = 180, int timeAfterFirstGoodAnswer = 30);
		Room(const Room& room);

	// Destructor
	public :
		~Room();

	// Operators
	public :
		Room& operator=(const Room &);
		friend QDataStream& operator<<(QDataStream& dataStream, Room room);
		friend QDataStream& operator>>(QDataStream& dataStream, Room& room);
};

Q_DECLARE_METATYPE(Room)

QDataStream& operator<<(QDataStream& dataStream, Room room);
QDataStream& operator>>(QDataStream& dataStream, Room& room);




#endif // ROOM_H
