#ifndef ROOM_H
#define ROOM_H



#include <QString>



class Room
{
    // Variables
    private :
        // Name of the room
        QString roomName;
        // Number of round by game
        int maxRounds;
        // Maximum number of players in the room
        int maxPlayers;
        // Time by round in second
        int timeByRound;
        // Time ramaining after one player have found the word
        int timeAfterFirstGoodAnswer;

    // Constructeur
    public:
        Room(QString roomName = "Default", int maxRounds = 10, int maxPlayers = 10, int timeByRound = 120, int timeAfterFirstGoodAnswer = 30);
};



#endif // ROOM_H