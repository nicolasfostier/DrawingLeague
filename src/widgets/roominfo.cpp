#include "include/widgets/roominfo.h"



// Setter
void RoomInfo::setRoom(Room room){
	name->setValue(room.getRoomName());
	this->setRound(room.getCurrentRound());
	this->setArtist(room.getArtist());
	this->setWord(room.getWord());
	this->setPointToWin(room.getPointToWin());

	this->timeByRound = room.getTimeByRound();
	this->timeAfterFirstGoodAnswer = room.getTimeAfterFirstGoodAnswer();

	this->timer->setTimeByRound(room.getTimeByRound());
	this->timer->setTimeAfterFirstGoodAnswer(room.getTimeAfterFirstGoodAnswer());
}
void RoomInfo::setRound(int round){
	if(round == 0){
		this->round->setValue(" ");
	}
	else{
		this->round->setValue(QString::number(round));
	}
}
void RoomInfo::setArtist(QString artist){
	this->artist->setValue(artist);
}
void RoomInfo::setWord(QString word){
	this->word->setValue(word);
}
void RoomInfo::setPointToWin(int pointToWin){
	if(pointToWin == 0){
		this->pointToWin->setValue(" ");
	}
	else{
		this->pointToWin->setValue(QString::number(pointToWin));
	}
}



// Getter
QString RoomInfo::getArtist(){
	return artist->getValue();
}
int RoomInfo::getPointToWin(){
	return pointToWin->getValue().toInt();
}
int RoomInfo::getTimeByRound(){
	return timeByRound;
}

int RoomInfo::getTimeAfterFirstGoodAnswer(){
	return timeAfterFirstGoodAnswer;
}



// Constructor
RoomInfo::RoomInfo(Room room, QWidget* parent) : QWidget(parent){
	this->setObjectName("RoomInfo");
	this->setStyleSheet("QWidget#RoomInfo{background: white; border: 1px solid grey;}");

	layout = new QGridLayout(this);
	layout->setContentsMargins(0,0,0,0);

		name = new RoomInfoPart(qApp->translate("RoomInfo", "Room name"), room.getRoomName(), this);
		layout->addWidget(name, 0, 0, 1, 1, Qt::AlignCenter);

		round = new RoomInfoPart(qApp->translate("RoomInfo", "Round"), " ", this);
		setRound(room.getCurrentRound());
		layout->addWidget(round, 0, 1, 1, 1, Qt::AlignCenter);

		artist = new RoomInfoPart(qApp->translate("RoomInfo", "Artist"), " ", this);
		setArtist(room.getWord());
		layout->addWidget(artist, 0, 2, 1, 1, Qt::AlignCenter);

		word = new RoomInfoPart(qApp->translate("RoomInfo", "Word"), " ", this);
		setWord(room.getWord());
		layout->addWidget(word, 0, 3, 1, 1, Qt::AlignCenter);

		pointToWin = new RoomInfoPart(qApp->translate("RoomInfo", "Point to win"), " ", this);
		setPointToWin(room.getPointToWin());
		layout->addWidget(pointToWin, 0, 4, 1, 1, Qt::AlignCenter);

		timer = new Timer(room.getTimeByRound(), room.getTimeAfterFirstGoodAnswer());
		layout->addWidget(timer, 0, 5, 1, 1, Qt::AlignCenter);
}



// Methods

void RoomInfo::timerStart(bool oneHasFound, int timeMs){
	if(timeMs == -2){
		if(oneHasFound){
			timeMs = timeAfterFirstGoodAnswer * 1000;
		}
		else{
			timeMs = timeByRound * 1000;
		}
	}
	timeMs += 1000;
	timer->start(timeMs, oneHasFound);
}

void RoomInfo::timerStop(){
	timer->stop();
}
