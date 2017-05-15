#include "include/room.h"



// Constructor
Room::Room(QString roomName, int maxRounds, int maxPlayers, int timeByRound, int timeAfterFirstGoodAnswer)
{
    this->roomName = roomName;
    this->maxRounds = maxRounds;
    this->maxPlayers = maxPlayers;
    this->timeByRound = timeByRound;
    this->timeAfterFirstGoodAnswer = timeAfterFirstGoodAnswer;
}
