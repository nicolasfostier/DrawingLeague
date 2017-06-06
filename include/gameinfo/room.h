#ifndef ROOM_H
#define ROOM_H



#include <QString>
#include <QTimer>
#include <QDataStream>



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

        //
        int round;
        //
        QString artist;
        //
        QString word;
        //
        int pointToWin;
        //
        int timeRemaining;

    // Getter
    public :
        QString getRoomName();
        int getMaxRounds();
        int getMaxPlayers();
        int getTimeByRound();
        int getTimeAfterFirstGoodAnswer();

        int getRound();
        QString getArtist();
        QString getWord();
        int getPointToWin();
        int getTimeRemaining();

    // Setter
    public :
        void setRoomName(QString roomName);
        void setMaxRounds(int maxRounds);
        void setMaxPlayers(int maxPlayers);
        void setTimeByRound(int timeByRound);
        void setTimeAfterFirstGoodAnswer(int timeAfterFirstGoodAnswer);

        void setRound(int round);
        void setArtist(QString artist);
        void setWord(QString word);
        void setPointToWin(int pointToWin);
        void setTimeRemaining(int timeRemaining);

    // Constructeur
    public :
        Room(QString roomName = " ", int maxRounds = 10, int maxPlayers = 10, int timeByRound = 90, int timeAfterFirstGoodAnswer = 15);
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

//
QDataStream& operator<<(QDataStream& dataStream, Room room);
//
QDataStream& operator>>(QDataStream& dataStream, Room& room);




#endif // ROOM_H
