#include "include/gameinfo/room.h"



// Getter
QString Room::getRoomName(){
    return roomName;
}
int Room::getMaxRounds(){
    return maxRounds;
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

int Room::getRound(){
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
    this->roomName = roomName;
}
void Room::setMaxRounds(int maxRounds){
    this->maxRounds = maxRounds;
}
void Room::setMaxPlayers(int maxPlayers){
    this->maxPlayers = maxPlayers;
}
void Room::setTimeByRound(int timeByRound){
    this->timeByRound = timeByRound;
}
void Room::setTimeAfterFirstGoodAnswer(int timeAfterFirstGoodAnswer){
    this->timeAfterFirstGoodAnswer = timeAfterFirstGoodAnswer;
}

void Room::setRound(int round){
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
Room::Room(QString roomName, int maxRounds, int maxPlayers, int timeByRound, int timeAfterFirstGoodAnswer){
    this->roomName = roomName;
    this->maxRounds = maxRounds;
    this->maxPlayers = maxPlayers;
    this->timeByRound = timeByRound;
    this->timeAfterFirstGoodAnswer = timeAfterFirstGoodAnswer;

    this->round = 0;
    this->artist = " ";
    this->word = " ";
    this->pointToWin = 0;
    this->timeRemaining = 0;
}
Room::Room(const Room& room){
    this->roomName = room.roomName;
    this->maxRounds = room.maxRounds;
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
    this->maxRounds = room.maxRounds;
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
    dataStream << room.maxRounds;
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
    dataStream >> room.maxRounds;
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
