#include "include/widgets/roominfo.h"



// Setter
void RoomInfo::setRoom(Room room){
    name->setValue(room.getRoomName());
    this->setRound(room.getRound());
    this->setArtist(room.getArtist());
    this->setWord(room.getWord());
    this->setPointToWin(room.getPointToWin());
    this->setTime(room.getTimeRemaining());
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
void RoomInfo::setTime(int timeRemaining, bool oneHasFound){
    if(oneHasFound){
        time->setValue("<b><span style='color: red; font-size: 16em;'>" + QString::number(timeRemaining / 60) + ":" + QString("%1").arg(timeRemaining % 60, 2, 10, QChar('0')) + "</span></b>");
    }
    else{
        time->setValue("<b><span style='color: black; font-size: 16em;'>" + QString::number(timeRemaining / 60) + ":" + QString("%1").arg(timeRemaining % 60, 2, 10, QChar('0')) + "</span></b>");
    }
}



// Constructor
RoomInfo::RoomInfo(Room room, QWidget* parent) : QWidget(parent)
{
    //
    this->setObjectName("RoomInfo");
    this->setStyleSheet("QWidget#RoomInfo{background: white; border: 1px solid grey;}");

    //
    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);

        //
        name = new RoomInfoPart(tr("Room name"), room.getRoomName(), this);
        layout->addWidget(name, 0, 0, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        round = new RoomInfoPart(tr("Round"), " ", this);
        setRound(room.getRound());
        layout->addWidget(round, 0, 1, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        artist = new RoomInfoPart(tr("Artist"), " ", this);
        setArtist(room.getWord());
        layout->addWidget(artist, 0, 2, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        word = new RoomInfoPart(tr("Word"), " ", this);
        setWord(room.getWord());
        layout->addWidget(word, 0, 3, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        pointToWin = new RoomInfoPart(tr("Point to win"), " ", this);
        setPointToWin(room.getPointToWin());
        layout->addWidget(pointToWin, 0, 4, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        time = new RoomInfoPart(tr("Time"), " ", this);
        setTime(room.getTimeRemaining());
        layout->addWidget(time, 0, 5, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);
}
