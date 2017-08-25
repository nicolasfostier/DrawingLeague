#include "include/socket/socketreader.h"



// Constructor
SocketReader::SocketReader(QTcpSocket *socket, QObject* parent) : QObject(parent){
	this->socket = socket;

	QObject::connect(socket, SIGNAL(readyRead()),
					 this, SLOT(read()));
}



// Destructor
SocketReader::~SocketReader(){

}



// Qt slots

void SocketReader::read(){
	while(socket->bytesAvailable()){
		BlockReader newBlock(socket);
		BlockType newBlockType;
		QDataStream& socketStream = newBlock.getStream();
		socketStream >> newBlockType;

		switch (newBlockType){
			case BlockType::NOTYPE : {

			break;
			}


			case BlockType::READY_TO_RECEIVE : {
				quint32 useless;
				socketStream >> useless;

				emit readyToReceive();
			break;
			}

			case BlockType::ENTER_THE_GAME : {
				Player player;
				QString gameVersion;
				socketStream >> player >> gameVersion;

				emit enterTheGameReceived(player, gameVersion);
			break;
			}

			case BlockType::HAS_ENTERED_THE_GAME : {
				quint32 useless;
				socketStream >> useless;

				emit hasEnteredTheGame();
			break;
			}

			case BlockType::GAME_ERROR : {
				ErrorCode errorCode;
				socketStream >> errorCode;

				emit gameErrorReceived(errorCode);
			break;
			}


			case BlockType::PLAYER_ENTERING : {
				Player player;
				socketStream >> player;

				emit playerEnteringReceived(player);
			break;
			}

			case BlockType::PLAYER_ONLINE : {
				Player player;
				socketStream >> player;

				emit playerOnlineReceived(player);
			break;
			}

			case BlockType::PLAYER_LEAVING : {
				Player player;
				socketStream >> player;

				emit playerLeavingReceived(player);
			break;
			}


			case BlockType::ROOM : {
				Room room;
				socketStream >> room;

				emit roomReceived(room);
			break;
			}


			case BlockType::GAME_STARTING : {
				quint32 useless;
				socketStream >> useless;

				emit gameStartingReceived();
			break;
			}

			case BlockType::ROUND_STARTING : {
				int round;
				QString artist;
				QString word;
				int pointToWin;
				socketStream >> round >> artist >> word >> pointToWin;

				emit roundStartingReceived(round, artist, word, pointToWin);
			break;
			}

			case BlockType::ROUND_ENDING : {
				QString word;
				socketStream >> word;

				emit roundEndingReceived(word);
			break;
			}

			case BlockType::GAME_ENDING : {
				QString winner;
				socketStream >> winner;

				emit gameEndingReceived(winner);
			break;
			}


			case BlockType::CHAT : {
				Message msg;
				socketStream >> msg;

				emit chatReceived(msg);
			break;
			}

			case BlockType::ANSWER : {
				Message msg;
				socketStream >> msg;

				emit answerReceived(msg);
			break;
			}

			case BlockType::ANSWER_FOUND : {
				QString pseudo;
				quint32 pointWon;
				socketStream >> pseudo >> pointWon;

				emit answerFoundReceived(pseudo, pointWon);
			break;
			}

			case BlockType::ANSWER_CLOSE : {
				quint32 useless;
				socketStream >> useless;

				emit answerCloseReceived();
			break;
			}


			case BlockType::HINT : {
				QString hint;
				socketStream >> hint;

				emit hintReceived(hint);
			break;
			}

			case BlockType::SKIP_WORD : {
				quint32 useless;
				socketStream >> useless;

				emit skipWordReceived();
			break;
			}


			case BlockType::READY : {
				quint32 useless;
				socketStream >> useless;

				emit readyReceived();
			break;
			}


			case BlockType::DRAWING_TOOL_TYPE : {
				DrawingToolType drawingToolType;
				socketStream >> drawingToolType;

				emit drawingToolTypeReceived(drawingToolType);
			break;
			}

			case BlockType::DRAWING_TOOL_COLOR : {
				QColor color;
				socketStream >> color;

				emit drawingToolColorReceived(color);
			break;
			}

			case BlockType::DRAWING_TOOL_WIDTH : {
				quint32 width;
				socketStream >> width;

				emit drawingToolWidthReceived(width);
			break;
			}


			case BlockType::CANVAS_RESET : {
				quint32 useless;
				socketStream >> useless;

				emit canvasResetReceived();
			break;
			}


			case BlockType::CANVAS_MOUSE_PRESS_EVENT : {
				QPoint pos;
				socketStream >> pos;

				emit canvasMousePressEventReceived(pos);
			break;
			}

			case BlockType::CANVAS_MOUSE_MOVE_EVENT : {
				QPoint pos;
				socketStream >> pos;

				emit canvasMouseMoveEventReceived(pos);
			break;
			}

			case BlockType::CANVAS_MOUSE_RELEASE_EVENT : {
				QPoint pos;
				socketStream >> pos;

				emit canvasMouseReleaseEventReceived(pos);
			break;
			}


			case BlockType::READY_NEEDED : {
				quint32 nbReadyNeeded;
				socketStream >> nbReadyNeeded;

				emit readyNeededReceived(nbReadyNeeded);
			break;
			}
		}
	}
}
