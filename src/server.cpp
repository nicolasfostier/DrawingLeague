#include "include/server.h"



// Constructor
Server::Server(Room room, int port)
{
    this->room = room;
    this->port = port;

    //
    this->listen(QHostAddress::Any, port);

    //
    QObject::connect(this, SIGNAL(newConnection()), this, SLOT(addClient()));

    // Move the object to another thread and start its execution
    QThread* threadCFU = new QThread();
    QObject::connect(threadCFU, SIGNAL(finished()), threadCFU, SLOT(deleteLater()));
    this->moveToThread(threadCFU);
    threadCFU->start();
}



// Destructor
Server::~Server(){
    // Delete its thread with it
    this->thread()->quit();
}


// Qt slots

//
void Server::addClient(){
    qDebug() << "addClient()";
    serverThreads.append(new ServerThread(nextPendingConnection(), &room, &serverThreads));
}

//
void Server::sendPlayersInfo(){

}
