#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H



#include <QThread>
#include <QTcpSocket>
#include <QDataStream>


//
#include "include/room.h"
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
        quint32 nextSizeToRead;
        //
        DataBlockType nextDataBlockType;

        //
        QString pseudo;
        //
        int score;
        //
        Room* room;
        //
        QList<ServerThread*>* serverThreads;

    // Constructeur
    public :
        ServerThread(QTcpSocket* socket, Room* room, QList<ServerThread*>* serverThreads);

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
        void sendNewPlayerInfo(QString msg);
        //
        void sendMsgChat(QString msg);
        //
        void sendMsgAnswer(QString msg);
};



#endif // SERVERTHREAD_H
