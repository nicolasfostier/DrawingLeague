#include "include/datablock/datablockwriter.h"



// Constructor
DataBlockWriter::DataBlockWriter(QTcpSocket* socket, QObject* parent) : QObject(parent)
{
	this->socket = socket;
	this->socketStream = new QDataStream(socket);
}



// Destructor
DataBlockWriter::~DataBlockWriter(){
	delete socketStream;
}



// Qt slots

void DataBlockWriter::sendReadyToReceive(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::READY_TO_RECEIVE << blockToSend;
}

void DataBlockWriter::sendEnterTheGame(Player player, QString gameVersion){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << player << gameVersion;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ENTER_THE_GAME << blockToSend;
}

void DataBlockWriter::sendHasEnteredTheGame(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::HAS_ENTERED_THE_GAME << blockToSend;
}

void DataBlockWriter::sendGameError(ErrorCode errorCode){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << errorCode;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::GAME_ERROR << blockToSend;
}


void DataBlockWriter::sendPlayerEntering(QString pseudo){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << pseudo;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::PLAYER_ENTERING << blockToSend;
}

void DataBlockWriter::sendPlayerOnline(Player player){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << player;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::PLAYER_ONLINE << blockToSend;
}

void DataBlockWriter::sendPlayerLeaving(QString pseudo){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << pseudo;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::PLAYER_LEAVING << blockToSend;
}


void DataBlockWriter::sendRoom(Room room){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << room;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ROOM << blockToSend;
}


void DataBlockWriter::sendGameStarting(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::GAME_STARTING << blockToSend;
}

void DataBlockWriter::sendRoundStarting(quint32 round, QString artist, QString word, quint32 pointToWin){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << round << artist << word << pointToWin;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ROUND_STARTING << blockToSend;
}

void DataBlockWriter::sendRoundEnding(QString word){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << word ;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ROUND_ENDING << blockToSend;
}

void DataBlockWriter::sendGameEnding(QString winner){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << winner;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::GAME_ENDING << blockToSend;
}


void DataBlockWriter::sendChat(Message msg){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << msg;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::CHAT << blockToSend;
}

void DataBlockWriter::sendAnswer(Message msg){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << msg;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ANSWER << blockToSend;
}

void DataBlockWriter::sendAnswerFound(QString pseudo, quint32 pointWon){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << pseudo << pointWon;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ANSWER_FOUND << blockToSend;
}

void DataBlockWriter::sendAnswerClose(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::ANSWER_CLOSE << blockToSend;
}


void DataBlockWriter::sendHint(QString hint){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << hint;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::HINT << blockToSend;
}

void DataBlockWriter::sendSkipWord(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::SKIP_WORD << blockToSend;
}


void DataBlockWriter::sendReady(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::READY << blockToSend;
}


void DataBlockWriter::sendDrawingToolType(DrawingToolType drawingToolType){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << drawingToolType;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::DRAWING_TOOL_TYPE << blockToSend;
}

void DataBlockWriter::sendDrawingToolColor(QColor color){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << color;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::DRAWING_TOOL_COLOR << blockToSend;
}

void DataBlockWriter::sendDrawingToolWidth(int width){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(width);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::DRAWING_TOOL_WIDTH << blockToSend;
}


void DataBlockWriter::sendCanvasReset(){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(0);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_RESET << blockToSend;
}


void DataBlockWriter::sendCanvasMousePressEvent(QPoint pos){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << pos;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_MOUSE_PRESS_EVENT << blockToSend;
}

void DataBlockWriter::sendCanvasMouseMoveEvent(QPoint pos){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << pos;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_MOUSE_MOVE_EVENT << blockToSend;
}

void DataBlockWriter::sendCanvasMouseReleaseEvent(QPoint pos){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << pos;
	*socketStream << quint32(blockToSend.size()) << DataBlockType::CANVAS_MOUSE_RELEASE_EVENT << blockToSend;
}


void DataBlockWriter::sendReadyNeeded(int nbReadyNeeded){
	QByteArray blockToSend;
	QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
	blockToSendStream << quint32(nbReadyNeeded);
	*socketStream << quint32(blockToSend.size()) << DataBlockType::READY_NEEDED << blockToSend;
}
