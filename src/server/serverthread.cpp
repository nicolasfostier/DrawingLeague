#include "include/server/serverthread.h"



// Getter
Player* ServerThread::getPlayer(){
    return player;
}
DataBlockReader* ServerThread::getDataBlockReader(){
    return dataBlockReader;
}
DataBlockWriter* ServerThread::getDataBlockWriter(){
    return dataBlockWriter;
}



// Constructeur
ServerThread::ServerThread(QTcpSocket* socket)
{
    this->player = NULL;
    this->socketDescriptor = socket->socketDescriptor();

    // Move the object to another thread and start its execution
    QThread* threadCFU = new QThread();
    QObject::connect(threadCFU, SIGNAL(finished()), threadCFU, SLOT(deleteLater()));
    this->moveToThread(threadCFU);
    QObject::connect(threadCFU, SIGNAL(started()), this, SLOT(launch()));
    this->thread()->start();
}



// Destructor
ServerThread::~ServerThread(){
    //
    emit playerLeaving(player->getPseudo());
    if(player != NULL){
        delete player;
    }

    //
    socket->deleteLater();
    delete dataBlockReader;
    delete dataBlockWriter;

    // Delete its thread with it
    this->thread()->quit();
}



// Qt slots

//
void ServerThread::launch(){
    //
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    //
    dataBlockReader = new DataBlockReader(socket);
    QObject::connect(dataBlockReader, SIGNAL(playerEnteringReceived(Player)), this, SLOT(setPlayer(Player)));

    //
    dataBlockWriter = new DataBlockWriter(socket);
}


//
void ServerThread::setPlayer(Player player){
    this->player = new Player(player);

    //
    emit readyToReceive(this);
}



// Operators
bool ServerThread::operator!=(ServerThread serverThread){
    return this->socket != serverThread.socket;
}
