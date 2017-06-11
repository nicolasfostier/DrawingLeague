#ifndef ROOMINFO_H
#define ROOMINFO_H



#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QApplication>


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
                //
                QFont timeFont;

    // Setter
    public :
        void setRoom(Room room);
        void setRound(int round);
        void setArtist(QString artist);
        void setWord(QString word);
        void setPointToWin(int pointToWin);
        void setTime(int timeRemaining, bool oneHasFound = false);

    // Constructor
    public :
        RoomInfo(Room room, QWidget* parent = 0);

    // Methods
    public :
        void updateTimeRemaining();
};



#endif // ROOMINFO_H
