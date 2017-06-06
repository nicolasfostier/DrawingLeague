#ifndef DATABLOCKWRITER_H
#define DATABLOCKWRITER_H



#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>


//
#include "include/datablock/datablocktype.h"
//
#include "include/gameinfo/message.h"
//
#include "include/gameinfo/room.h"
//
#include "include/gameinfo/player.h"
//
#include "include/drawingtooltype.h"



//
class DataBlockWriter : public QObject
{
    Q_OBJECT

    // Variables
    private :
        //
        QTcpSocket* socket;
        //
        QDataStream* socketStream;

    // Constructor
    public :
        DataBlockWriter(QTcpSocket* socket);

    // Destructor
    public :
        ~DataBlockWriter();

    // Qt slots
    public slots :
        //
        void sendRoom(Room room);

        //
        void sendPlayerEntering(Player player);
        //
        void sendPlayerLeaving(QString pseudo);

        //
        void sendRoundStarting(quint32 round, QString artist, QString word, quint32 pointToWin);

        //
        void sendAnswerFound(QString pseudo, quint32 pointWon);

        //
        void sendAnswer(Message msg);
        //
        void sendChat(Message msg);

        //
        void sendDrawingToolType(DrawingToolType drawingToolType);
        //
        void sendDrawingToolColor(QColor color);
        //
        void sendDrawingToolWidth(int width);

        //
        void sendCanvasReset();

        //
        void sendCanvasMousePressEvent(QPoint pos);
        //
        void sendCanvasMouseMoveEvent(QPoint pos);
        //
        void sendCanvasMouseReleaseEvent(QPoint pos);

        //
        void sendServerMsgTypeReady();
        //
        void sendServerMsgReadyNeeded(int howManyMoreReadyNeeded);
};



#endif // DATABLOCKWRITER_H
