#ifndef SERVER_H
#define SERVER_H



#include <QTcpServer>
#include <QThread>

//
#include "include/gameinfo/room.h"
//
#include "include/server/serverthread.h"



// Host a room
class Server : public QTcpServer
{
    Q_OBJECT

    // Variables
    private :
        //
        Room room;
        //
        int port;
        //
        QList<ServerThread*> serverThreads;

    // Constructor
    public:
        Server(Room room, int port);

    // Destructor
    public :
        ~Server();

    // Qt slots
    public slots :
        //
        void addClient();

        //
        void sendPlayersInfo();
};

#endif // SERVER_H
