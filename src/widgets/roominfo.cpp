#include "include/widgets/roominfo.h"



// Setter
void RoomInfo::updateRoom(Room room){
    name->setValue(room.getRoomName());
    round->setValue(QString::number(room.getRound()));
    artist->setValue(room.getArtist());
    word->setValue(room.getWord());
    pointToWin->setValue(QString::number(room.getPointToWin()));
    time->setValue(" ");
}
void RoomInfo::setRound(int round){
    this->round->setValue(QString::number(round));
}
void RoomInfo::setArtist(QString artist){
    this->artist->setValue(artist);
}
void RoomInfo::setWord(QString word){
    this->word->setValue(word);
}
void RoomInfo::setPointToWin(int pointToWin){
    this->pointToWin->setValue(QString::number(pointToWin));
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
        round = new RoomInfoPart(tr("Round"), QString::number(room.getRound()), this);
        layout->addWidget(round, 0, 1, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        artist = new RoomInfoPart(tr("Artist"), room.getArtist(), this);
        layout->addWidget(artist, 0, 2, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        word = new RoomInfoPart(tr("Word"), " ", this);
        layout->addWidget(word, 0, 3, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        pointToWin = new RoomInfoPart(tr("Point to win"), " ", this);
        layout->addWidget(pointToWin, 0, 4, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);

        //
        time = new RoomInfoPart(tr("Time"), " ", this);
        layout->addWidget(time, 0, 5, 1, 1, Qt::AlignVCenter | Qt::AlignCenter);
}
