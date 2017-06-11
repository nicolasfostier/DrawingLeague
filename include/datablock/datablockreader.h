#ifndef DATABLOCK_H
#define DATABLOCK_H



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
class DataBlockReader : public QObject
{
    Q_OBJECT

    // Variables
    private :
        //
        QTcpSocket* socket;
        //
        QDataStream* socketStream;

        //
        int nextSizeToRead;
        //
        DataBlockType nextDataBlockType;

    // Constructor
    public :
        DataBlockReader(QTcpSocket* socket);

    // Destructor
    public :
        ~DataBlockReader();

    // Qt slots
    public slots :
        //
        void read();

    // Signals
    signals :
        //
        void readyToReceive();

        //
        void pseudoOk();
        //
        void pseudoAlreadyUsed();

        //
        void playerEnteringReceived(Player player);
        //
        void playerOnlineReceived(Player pseudo);
        //
        void playerLeavingReceived(QString pseudo);

        //
        void roomReceived(Room room);

        //
        void gameStartingReceived();
        //
        void roundStartingReceived(int round, QString artist, QString word, int pointToWin);
        //
        void roundEndingReceived(QString word);
        //
        void skipWordReceived();
        //
        void gameEndingReceived(QString winner);

        //
        void answerFoundReceived(QString pseudo, int pointWon);

        //
        void answerReceived(Message msg);
        //
        void chatReceived(Message msg);

        //
        void drawingToolTypeReceived(DrawingToolType drawingToolType);
        //
        void drawingToolColorReceived(QColor color);
        //
        void drawingToolWidthReceived(int width);

        //
        void canvasResetReceived(bool confirmation = false);

        //
        void canvasMousePressEventReceived(QPoint pos);
        //
        void canvasMouseMoveEventReceived(QPoint pos);
        //
        void canvasMouseReleaseEventReceived(QPoint pos);

        //
        void serverMsgReadyNeededReceived(int howManyMoreReadyNeeded);

        //
        void serverClosed();
};



#endif // DATABLOCK_H
