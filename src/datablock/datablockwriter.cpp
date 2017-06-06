#include "include/datablock/datablockwriter.h"



// Constructor
DataBlockWriter::DataBlockWriter(QTcpSocket* socket)
{
    this->socket = socket;
    this->socketStream = new QDataStream(socket);
}



// Destructor
DataBlockWriter::~DataBlockWriter(){
    delete socketStream;
}



// Qt slots

//
void DataBlockWriter::sendRoom(Room room){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << room;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::ROOM << blockToSend;
}


//
void DataBlockWriter::sendPlayerEntering(Player player){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << player;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::PLAYER_ENTERING << blockToSend;
}

//
void DataBlockWriter::sendPlayerLeaving(QString pseudo){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << pseudo;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::PLAYER_LEAVING << blockToSend;
}


//
void DataBlockWriter::sendRoundStarting(quint32 round, QString artist, QString word, quint32 pointToWin){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << round << artist << word << pointToWin;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::ROUND_STARTING << blockToSend;
}


//
void DataBlockWriter::sendAnswerFound(QString pseudo, quint32 pointWon){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << pseudo << pointWon;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::ANSWER_FOUND << blockToSend;
}


//
void DataBlockWriter::sendAnswer(Message msg){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << msg;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::ANSWER << blockToSend;
}

//
void DataBlockWriter::sendChat(Message msg){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << msg;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::CHAT << blockToSend;
}


//
void DataBlockWriter::sendDrawingToolType(DrawingToolType drawingToolType){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << drawingToolType;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::DRAWING_TOOL_TYPE << blockToSend;
}

//
void DataBlockWriter::sendDrawingToolColor(QColor color){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << color;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::DRAWING_TOOL_COLOR << blockToSend;
}

//
void DataBlockWriter::sendDrawingToolWidth(int width){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << quint32(width);
    *socketStream << quint32(blockToSend.size()) << DataBlockType::DRAWING_TOOL_WIDTH << blockToSend;
}


//
void DataBlockWriter::sendCanvasReset(){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << quint32(0);
    *socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_RESET << blockToSend;
}


//
void DataBlockWriter::sendCanvasMousePressEvent(QPoint pos){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << pos;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_MOUSE_PRESS_EVENT << blockToSend;
}

//
void DataBlockWriter::sendCanvasMouseMoveEvent(QPoint pos){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << pos;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_MOUSE_MOVE_EVENT << blockToSend;
}

//
void DataBlockWriter::sendCanvasMouseReleaseEvent(QPoint pos){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << pos;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_MOUSE_RELEASE_EVENT << blockToSend;
}


//
void DataBlockWriter::sendServerMsgTypeReady(){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << quint32(0);
    *socketStream << quint32(blockToSend.size()) << DataBlockType::SERVER_MSG_TYPE_READY << blockToSend;
}

//
void DataBlockWriter::sendServerMsgReadyNeeded(int howManyMoreReadyNeeded){
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << quint32(howManyMoreReadyNeeded);
    *socketStream << quint32(blockToSend.size()) << DataBlockType::SERVER_MSG_READY_NEEDED << blockToSend;
}
