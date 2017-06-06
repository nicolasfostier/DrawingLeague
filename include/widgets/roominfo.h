#ifndef ROOMINFO_H
#define ROOMINFO_H



#include <QWidget>
#include <QGridLayout>

//
#include "include/gameinfo/room.h"
//
#include "include/widgets/roominfopart.h"



//
class RoomInfo : public QWidget
{
    // Variable
    private :

        //
        QGridLayout* layout;
            //
            RoomInfoPart* name;
            //
            RoomInfoPart* round;
            //
            RoomInfoPart* artist;
            //
            RoomInfoPart* word;
            //
            RoomInfoPart* pointToWin;
            //
            RoomInfoPart* time;

    // Setter
    public :
        void updateRoom(Room room);
        void setRound(int round);
        void setArtist(QString artist);
        void setWord(QString word);
        void setPointToWin(int pointToWin);
        void setTime(int timeRemaining);

    // Constructor
    public:
        RoomInfo(Room room, QWidget* parent = 0);
};



#endif // ROOMINFO_H
