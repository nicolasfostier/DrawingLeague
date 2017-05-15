#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H



#include <QThread>
#include <QTcpSocket>
#include <QDataStream>


//
#include "include/gameinfo/room.h"
//
#include "include/datablock.h"



//
class ServerThread : public QObject
{
    Q_OBJECT

    // Variables
    private :
        //
        int socketDescriptor;
        //
        QTcpSocket* socket;
        //
        QDataStream* socketStream;
        //
        int nextSizeToRead;
        //
        DataBlockType nextDataBlockType;

        //
        QString pseudo;
        //
        int score;
        //
        Room* room;

    // Constructeur
    public :
        ServerThread(QTcpSocket* socket, Room* room);

    // Destructor
    public :
        ~ServerThread();

    // Qt slots
    public slots :
        //
        void launch();
        //
        void readFromClient();

        //
        void sendNewPlayerInfo(QString pseudo);
        //
        void sendMsgChat(QString msg);
        //
        void sendMsgAnswer(QString msg);

    // Qt signals
    signals :
        //
        void sendNewPlayerInfoSignal(QString pseudo);
        //
        void sendMsgChatSignal(QString msg);
        //
        void sendMsgAnswerSignal(QString msg);
};



#endif // SERVERTHREAD_H
