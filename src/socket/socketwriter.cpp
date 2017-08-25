#include "include/socket/socketwriter.h"



// Constructor
SocketWriter::SocketWriter(QTcpSocket* socket, QObject* parent) : QObject(parent){
	this->socket = socket;
}



// Destructor
SocketWriter::~SocketWriter(){

}



// Qt slots

void SocketWriter::sendReadyToReceive(){
	BlockWriter(socket).getStream() << BlockType::READY_TO_RECEIVE
									<< quint32(0);
}

void SocketWriter::sendEnterTheGame(Player player, QString gameVersion){
	BlockWriter(socket).getStream()	<< BlockType::ENTER_THE_GAME
									<< player
									<< gameVersion;
}

void SocketWriter::sendHasEnteredTheGame(){
	BlockWriter(socket).getStream() << BlockType::HAS_ENTERED_THE_GAME
									<< quint32(0);
}

void SocketWriter::sendGameError(ErrorCode errorCode){
	BlockWriter(socket).getStream() << BlockType::GAME_ERROR
									<< errorCode;
}


void SocketWriter::sendPlayerEntering(Player player){
	BlockWriter(socket).getStream() << BlockType::PLAYER_ENTERING
									<< player;
}

void SocketWriter::sendPlayerOnline(Player player){
	BlockWriter(socket).getStream() << BlockType::PLAYER_ONLINE
									<< player;
}

void SocketWriter::sendPlayerLeaving(Player player){
	BlockWriter(socket).getStream() << BlockType::PLAYER_LEAVING
									<< player;
}


void SocketWriter::sendRoom(Room room){
	BlockWriter(socket).getStream() << BlockType::ROOM
									<< room;
}


void SocketWriter::sendGameStarting(){
	BlockWriter(socket).getStream() << BlockType::GAME_STARTING
									<< quint32(0);
}

void SocketWriter::sendRoundStarting(quint32 round, QString artist, QString word, quint32 pointToWin){
	BlockWriter(socket).getStream() << BlockType::ROUND_STARTING
									<< round
									<< artist
									<< word
									<< pointToWin;
}

void SocketWriter::sendRoundEnding(QString word){
	BlockWriter(socket).getStream() << BlockType::ROUND_ENDING
									<< word;
}

void SocketWriter::sendGameEnding(QString winner){
	BlockWriter(socket).getStream() << BlockType::GAME_ENDING
									<< winner;
}


void SocketWriter::sendChat(Message msg){
	BlockWriter(socket).getStream() << BlockType::CHAT
									<< msg;
}

void SocketWriter::sendAnswer(Message msg){
	BlockWriter(socket).getStream() << BlockType::ANSWER
									<< msg;
}

void SocketWriter::sendAnswerFound(QString pseudo, quint32 pointWon){
	BlockWriter(socket).getStream() << BlockType::ANSWER_FOUND
									<< pseudo
									<< pointWon;
}

void SocketWriter::sendAnswerClose(){
	BlockWriter(socket).getStream() << BlockType::ANSWER_CLOSE
									<< quint32(0);
}


void SocketWriter::sendHint(QString hint){
	BlockWriter(socket).getStream() << BlockType::HINT
									<< hint;
}

void SocketWriter::sendSkipWord(){
	BlockWriter(socket).getStream() << BlockType::SKIP_WORD
									<< quint32(0);
}


void SocketWriter::sendReady(){
	BlockWriter(socket).getStream() << BlockType::READY
									<< quint32(0);
}


void SocketWriter::sendDrawingToolType(DrawingToolType drawingToolType){
	BlockWriter(socket).getStream() << BlockType::DRAWING_TOOL_TYPE
									<< drawingToolType;
}

void SocketWriter::sendDrawingToolColor(QColor color){
	BlockWriter(socket).getStream() << BlockType::DRAWING_TOOL_COLOR
									<< color;
}

void SocketWriter::sendDrawingToolWidth(int width){
	BlockWriter(socket).getStream() << BlockType::DRAWING_TOOL_WIDTH
									<< quint32(width);
}


void SocketWriter::sendCanvasReset(){
	BlockWriter(socket).getStream() << BlockType::CANVAS_RESET
									<< quint32(0);
}


void SocketWriter::sendCanvasMousePressEvent(QPoint pos){
	BlockWriter(socket).getStream() << BlockType::CANVAS_MOUSE_PRESS_EVENT
									<< pos;
}

void SocketWriter::sendCanvasMouseMoveEvent(QPoint pos){
	BlockWriter(socket).getStream() << BlockType::CANVAS_MOUSE_MOVE_EVENT
									<< pos;
}

void SocketWriter::sendCanvasMouseReleaseEvent(QPoint pos){
	BlockWriter(socket).getStream() << BlockType::CANVAS_MOUSE_RELEASE_EVENT
									<< pos;
}


void SocketWriter::sendReadyNeeded(int nbReadyNeeded){
	BlockWriter(socket).getStream() << BlockType::READY_NEEDED
									<< quint32(nbReadyNeeded);
}
