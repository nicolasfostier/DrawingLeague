#include "include/gameinfo/room.h"



// Getter
QString Room::getRoomName(){
	return roomName;
}
int Room::getNumberOfRounds(){
	return numberOfRounds;
}
int Room::getMaxPlayers(){
	return maxPlayers;
}
int Room::getTimeByRound(){
	return timeByRound;
}
int Room::getTimeAfterFirstGoodAnswer(){
	return timeAfterFirstGoodAnswer;
}

int Room::getCurrentRound(){
	return round;
}
QString Room::getArtist(){
	return artist;
}
QString Room::getWord(){
	return word;
}
int Room::getPointToWin(){
	return pointToWin;
}
int Room::getTimeRemaining(){
	return timeRemaining;
}



// Setter
void Room::setRoomName(QString roomName){
	if(roomName.size() < 3){
		qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
					<< "The name's room must be at least 3 characters long."
					<< roomName;
		throw std::invalid_argument(tr("The name's room must be at least 3 characters long.").toUtf8());
	}
	else{
		this->roomName = roomName;
	}
}
void Room::setNumberOfRounds(int numberofRounds){
	if(numberofRounds < 1){
		qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
					<< "The number of rounds must be at least 1."
					<< numberofRounds;
		throw std::invalid_argument(tr("The number of rounds must be at least 1.").toUtf8());
	}
	else{
		this->numberOfRounds = numberofRounds;
	}
}
void Room::setMaxPlayers(int maxPlayers){
	if(maxPlayers < 2){
		qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
					<< "The maximum number of players must be at least 2."
					<< maxPlayers;
		throw std::invalid_argument("The maximum number of players must be at least 2.");
	}
	else{
		this->maxPlayers = maxPlayers;
	}
}
void Room::setTimeByRound(int timeByRound){
	if(timeByRound < 5){
		qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
					<< "The duration of a round must be at least 5 seconds."
					<< timeByRound;
		throw std::invalid_argument("The duration of a round must be at least 5 seconds.");
	}
	else{
		this->timeByRound = timeByRound;
	}
}
void Room::setTimeAfterFirstGoodAnswer(int timeAfterFirstGoodAnswer){
	if(timeAfterFirstGoodAnswer < 0){
		qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
					<< "The time remaining after the first good answer can't be negative."
					<< timeAfterFirstGoodAnswer;
		throw std::invalid_argument("The time remaining after the first good answer can't be negative.");
	}
	else{
		this->timeAfterFirstGoodAnswer = timeAfterFirstGoodAnswer;
	}
}

void Room::setCurrentRound(int round){
	this->round = round;
}
void Room::setArtist(QString artist){
	this->artist = artist;
}
void Room::setWord(QString word){
	this->word = word;
}
void Room::setPointToWin(int pointToWin){
	this->pointToWin = pointToWin;
}
void Room::setTimeRemaining(int timeRemaining){
	this->timeRemaining = timeRemaining;
}



// Constructor
Room::Room(QString roomName, int numberofRounds, int maxPlayers, int timeByRound, int timeAfterFirstGoodAnswer) : QObject(){
	this->setRoomName(roomName);
	this->setNumberOfRounds(numberofRounds);
	this->setMaxPlayers(maxPlayers);
	this->setTimeByRound(timeByRound);
	this->setTimeAfterFirstGoodAnswer(timeAfterFirstGoodAnswer);

	this->round = 0;
	this->artist = " ";
	this->word = " ";
	this->pointToWin = 0;
	this->timeRemaining = 0;
}
Room::Room(const Room& room) : QObject(){
	this->roomName = room.roomName;
	this->numberOfRounds = room.numberOfRounds;
	this->maxPlayers = room.maxPlayers;
	this->timeByRound = room.timeByRound;
	this->timeAfterFirstGoodAnswer = room.timeAfterFirstGoodAnswer;

	this->round = room.round;
	this->artist = room.artist;
	this->word = room.word;
	this->pointToWin = room.pointToWin;
	this->timeRemaining = room.timeRemaining;
}



// Destructor
Room::~Room(){

}



// Operators
Room& Room::operator=(const Room & room){
	this->roomName = room.roomName;
	this->numberOfRounds = room.numberOfRounds;
	this->maxPlayers = room.maxPlayers;
	this->timeByRound = room.timeByRound;
	this->timeAfterFirstGoodAnswer = room.timeAfterFirstGoodAnswer;

	this->round = room.round;
	this->artist = room.artist;
	this->word = room.word;
	this->pointToWin = room.pointToWin;
	this->timeRemaining = room.timeRemaining;

	return *this;
}
//
QDataStream& operator<<(QDataStream& dataStream, Room room){
	dataStream << room.roomName;
	dataStream << room.numberOfRounds;
	dataStream << room.maxPlayers;
	dataStream << room.timeByRound;
	dataStream << room.timeAfterFirstGoodAnswer;

	dataStream << room.round;
	dataStream << room.artist;
	dataStream << room.word;
	dataStream << room.pointToWin;
	dataStream << room.timeRemaining;

	return dataStream;
}
//
QDataStream& operator>>(QDataStream& dataStream, Room& room){
	dataStream >> room.roomName;
	dataStream >> room.numberOfRounds;
	dataStream >> room.maxPlayers;
	dataStream >> room.timeByRound;
	dataStream >> room.timeAfterFirstGoodAnswer;

	dataStream >> room.round;
	dataStream >> room.artist;
	dataStream >> room.word;
	dataStream >> room.pointToWin;
	dataStream >> room.timeRemaining;

	return dataStream;
}
