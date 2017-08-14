#ifndef ROOMINFO_H
#define ROOMINFO_H



#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QApplication>


#include "include/gameinfo/room.h"
#include "include/widgets/roominfopart.h"
#include "include/widgets/timer.h"



// Display all the information about the current room
class RoomInfo : public QWidget
{
	// Variable
	private :
		QGridLayout* layout;
			RoomInfoPart* name;
			RoomInfoPart* round;
			RoomInfoPart* artist;
			RoomInfoPart* word;
			RoomInfoPart* pointToWin;
			RoomInfoPart* time;
				QFont timeFont;
			Timer* timer;
				int timeByRound;
				int timeAfterFirstGoodAnswer;

	// Setter
	public :
		void setRoom(Room room);
		void setRound(int round);
		void setArtist(QString artist);
		void setWord(QString word);
		void setPointToWin(int pointToWin);

	// Getter
	public :
		QString getArtist();
		int getPointToWin();
		int getTimeByRound();
		int getTimeAfterFirstGoodAnswer();

	// Constructor
	public :
		RoomInfo(Room room, QWidget* parent = 0);

	// Methods
	public :
		void timerStart(bool oneHasFound, int timeMs = -2);
		void timerStop();
};



#endif // ROOMINFO_H
