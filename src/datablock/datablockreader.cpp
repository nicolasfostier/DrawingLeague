#include "include/datablock/datablockreader.h"



// Constructor
DataBlockReader::DataBlockReader(QTcpSocket *socket, QObject* parent) : QObject(parent){
	this->socket = socket;
	this->socketStream = new QDataStream(socket);

	this->nextSizeToRead = 0;
	this->nextDataBlockType = DataBlockType::NOTYPE;

	QObject::connect(socket, SIGNAL(readyRead()),
					 this, SLOT(read()));
}



// Destructor
DataBlockReader::~DataBlockReader(){
	delete socketStream;
}



// Qt slots

void DataBlockReader::read(){
//	qInfo() << "read" << this->parent();
	while(1){
		if(nextSizeToRead == 0){
			if(socket->bytesAvailable() < qint64(sizeof(quint32))){
				break;
			}
			*socketStream >> nextSizeToRead;
		}

		if(nextDataBlockType == DataBlockType::NOTYPE){
			//
			if(socket->bytesAvailable() < qint64(sizeof(quint16))){
				break;
			}
			*socketStream >> nextDataBlockType;
		}

		QByteArray blockReceived;
		QDataStream blockReceivedStream(&blockReceived, QIODevice::ReadWrite);

		switch (nextDataBlockType){
			case DataBlockType::NOTYPE : {

			break;
			}


			case DataBlockType::READY_TO_RECEIVE : {
				qInfo() << "DBR ready to receive";

				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit readyToReceive();
			break;
			}

			case DataBlockType::ENTER_THE_GAME : {
				Player player;
				QString gameVersion;
				*socketStream >> blockReceived;
				blockReceivedStream >> player >> gameVersion;

				qInfo() << "DBR enter the game";

				emit enterTheGameReceived(player, gameVersion);
			break;
			}

			case DataBlockType::HAS_ENTERED_THE_GAME : {
				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit hasEnteredTheGame();
			break;
			}

			case DataBlockType::GAME_ERROR : {
				ErrorCode errorCode;
				*socketStream >> blockReceived;
				blockReceivedStream >> errorCode;

				emit gameErrorReceived(errorCode);
			break;
			}


			case DataBlockType::PLAYER_ENTERING : {
				QString pseudo;
				*socketStream >> blockReceived;
				blockReceivedStream >> pseudo;

				emit playerEnteringReceived(pseudo);
			break;
			}

			case DataBlockType::PLAYER_ONLINE : {
				Player player;
				*socketStream >> blockReceived;
				blockReceivedStream >> player;

				emit playerOnlineReceived(player);
			break;
			}

			case DataBlockType::PLAYER_LEAVING : {
				QString pseudo;
				*socketStream >> blockReceived;
				blockReceivedStream >> pseudo;

				emit playerLeavingReceived(pseudo);
			break;
			}


			case DataBlockType::ROOM : {
				Room room;
				*socketStream >> blockReceived;
				blockReceivedStream >> room;

				emit roomReceived(room);
			break;
			}


			case DataBlockType::GAME_STARTING : {
				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit gameStartingReceived();
			break;
			}

			case DataBlockType::ROUND_STARTING : {
				int round;
				QString artist;
				QString word;
				int pointToWin;
				*socketStream >> blockReceived;
				blockReceivedStream >> round >> artist >> word >> pointToWin;

				emit roundStartingReceived(round, artist, word, pointToWin);
			break;
			}

			case DataBlockType::ROUND_ENDING : {
				QString word;
				*socketStream >> blockReceived;
				blockReceivedStream >> word;

				emit roundEndingReceived(word);
			break;
			}

			case DataBlockType::GAME_ENDING : {
				QString winner;
				*socketStream >> blockReceived;
				blockReceivedStream >> winner;

				emit gameEndingReceived(winner);
			break;
			}


			case DataBlockType::CHAT : {
				Message msg;
				*socketStream >> blockReceived;
				blockReceivedStream >> msg;

				emit chatReceived(msg);
			break;
			}

			case DataBlockType::ANSWER : {
				Message msg;
				*socketStream >> blockReceived;
				blockReceivedStream >> msg;

				emit answerReceived(msg);
			break;
			}

			case DataBlockType::ANSWER_FOUND : {
				QString pseudo;
				quint32 pointWon;
				*socketStream >> blockReceived;
				blockReceivedStream >> pseudo >> pointWon;

				emit answerFoundReceived(pseudo, pointWon);
			break;
			}

			case DataBlockType::ANSWER_CLOSE : {
				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit answerCloseReceived();
			break;
			}


			case DataBlockType::HINT : {
				QString hint;
				*socketStream >> blockReceived;
				blockReceivedStream >> hint;

				emit hintReceived(hint);
			break;
			}

			case DataBlockType::SKIP_WORD : {
				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit skipWordReceived();
			break;
			}


			case DataBlockType::READY : {
				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit readyReceived();
			break;
			}


			case DataBlockType::DRAWING_TOOL_TYPE : {
				DrawingToolType drawingToolType;
				*socketStream >> blockReceived;
				blockReceivedStream >> drawingToolType;

				emit drawingToolTypeReceived(drawingToolType);
			break;
			}

			case DataBlockType::DRAWING_TOOL_COLOR : {
				QColor color;
				*socketStream >> blockReceived;
				blockReceivedStream >> color;

				emit drawingToolColorReceived(color);
			break;
			}

			case DataBlockType::DRAWING_TOOL_WIDTH : {
				quint32 width;
				*socketStream >> blockReceived;
				blockReceivedStream >> width;

				emit drawingToolWidthReceived(width);
			break;
			}


			case DataBlockType::CANVAS_RESET : {
				quint32 useless;
				*socketStream >> blockReceived;
				blockReceivedStream >> useless;

				emit canvasResetReceived();
			break;
			}


			case DataBlockType::CANVAS_MOUSE_PRESS_EVENT : {
				QPoint pos;
				*socketStream >> blockReceived;
				blockReceivedStream >> pos;

				emit canvasMousePressEventReceived(pos);
			break;
			}

			case DataBlockType::CANVAS_MOUSE_MOVE_EVENT : {
				QPoint pos;
				*socketStream >> blockReceived;
				blockReceivedStream >> pos;

				emit canvasMouseMoveEventReceived(pos);
			break;
			}

			case DataBlockType::CANVAS_MOUSE_RELEASE_EVENT : {
				QPoint pos;
				*socketStream >> blockReceived;
				blockReceivedStream >> pos;

				emit canvasMouseReleaseEventReceived(pos);
			break;
			}


			case DataBlockType::READY_NEEDED : {
				quint32 nbReadyNeeded;
				*socketStream >> blockReceived;
				blockReceivedStream >> nbReadyNeeded;

				emit readyNeededReceived(nbReadyNeeded);
			break;
			}
		}

		nextSizeToRead = 0;
		nextDataBlockType = DataBlockType::NOTYPE;
	}
}
