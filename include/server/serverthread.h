#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H



#include <QThread>
#include <QTcpSocket>
#include <QDataStream>


//
#include "include/gameinfo/room.h"
//
#include "include/datablock/datablockreader.h"
//
#include "include/datablock/datablockwriter.h"
//
#include "include/gameinfo/message.h"
//
#include "include/gameinfo/player.h"



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
        DataBlockReader* dataBlockReader;
        //
        DataBlockWriter* dataBlockWriter;

        //
        Player* player;

    // Getter
    public :
        Player* getPlayer();
        DataBlockReader* getDataBlockReader();
        DataBlockWriter* getDataBlockWriter();

    // Constructeur
    public :
        ServerThread(QTcpSocket* socket);

    // Destructor
    public :
        ~ServerThread();

    // Qt slots
    public slots :
        //
        void launch();

        //
        void setPlayer(Player player);

    // Qt signals
    signals :
        //
        void readyToReceive(ServerThread* serverThread);

        //
        void playerLeaving(QString pseudo);

    // Operators
    public :
        bool operator!=(ServerThread serverThread);
};



#endif // SERVERTHREAD_H
