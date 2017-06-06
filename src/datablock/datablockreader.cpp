#include "include/datablock/datablockreader.h"



// Constructor
DataBlockReader::DataBlockReader(QTcpSocket *socket){
    this->socket = socket;
    this->socketStream = new QDataStream(socket);
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(read()));

    this->nextSizeToRead = 0;
    this->nextDataBlockType = DataBlockType::NOTYPE;
}



// Destructor
DataBlockReader::~DataBlockReader(){
    delete socketStream;
}



// Qt slots

//
void DataBlockReader::read(){
    //
    while(1){
        //
        if(nextSizeToRead == 0){
            //
            if(socket->bytesAvailable() < sizeof(quint32)){
                break;
            }
            *socketStream >> nextSizeToRead;
        }

        //
        if(nextDataBlockType == DataBlockType::NOTYPE){
            //
            if(socket->bytesAvailable() < sizeof(quint16)){
                break;
            }
            *socketStream >> nextDataBlockType;
        }

        QByteArray blockReceived;
        QDataStream blockReceivedStream(&blockReceived, QIODevice::ReadWrite);

        switch (nextDataBlockType){
            case DataBlockType::ROOM : {
                Room room;
                *socketStream >> blockReceived;
                blockReceivedStream >> room;

                emit roomReceived(room);
            break;
            }


            case DataBlockType::PLAYER_ENTERING : {
                Player player;
                *socketStream >> blockReceived;
                blockReceivedStream >> player;

                emit playerEnteringReceived(player);
            break;
            }

            case DataBlockType::PLAYER_LEAVING : {
                QString pseudo;
                *socketStream >> blockReceived;
                blockReceivedStream >> pseudo;

                emit playerLeavingReceived(pseudo);
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


            case DataBlockType::ANSWER_FOUND : {
                QString pseudo;
                quint32 pointWon;
                *socketStream >> blockReceived;
                blockReceivedStream >> pseudo >> pointWon;

                emit answerFoundReceived(pseudo, pointWon);
            break;
            }


            case DataBlockType::ANSWER : {
                Message msg;
                *socketStream >> blockReceived;
                blockReceivedStream >> msg;

                emit answerReceived(msg);
            break;
            }

            case DataBlockType::CHAT : {
                Message msg;
                *socketStream >> blockReceived;
                blockReceivedStream >> msg;

                emit chatReceived(msg);
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


            case DataBlockType::SERVER_MSG_TYPE_READY : {
                quint32 useless;
                *socketStream >> blockReceived;
                blockReceivedStream >> useless;

                emit serverMsgTypeReadyReceived();
            break;
            }

            case DataBlockType::SERVER_MSG_READY_NEEDED : {
                quint32 howManyMoreReadyNeeded;
                *socketStream >> blockReceived;
                blockReceivedStream >> howManyMoreReadyNeeded;

                emit serverMsgReadyNeededReceived(howManyMoreReadyNeeded);
            break;
            }
        }

        nextSizeToRead = 0;
        nextDataBlockType = DataBlockType::NOTYPE;
    }
}
