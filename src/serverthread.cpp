#include "include/serverthread.h"



// Constructeur
ServerThread::ServerThread(QTcpSocket* socket, Room* room, QList<ServerThread*>* serverThreads)
{
    this->socketDescriptor = socket->socketDescriptor();
    this->room = room;
    this->serverThreads = serverThreads;

    // Move the object to another thread and start its execution
    QThread* threadCFU = new QThread();
    QObject::connect(threadCFU, SIGNAL(finished()), threadCFU, SLOT(deleteLater()));
    this->moveToThread(threadCFU);
    QObject::connect(threadCFU, SIGNAL(started()), this, SLOT(launch()));
    threadCFU->start();
}



// Destructor
ServerThread::~ServerThread(){
    // Delete its thread with it
    this->thread()->quit();
}



// Qt slots

//
void ServerThread::launch(){
    qDebug() << "New player !";

    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    socketStream = new QDataStream(socket);
    nextSizeToRead = 0;
    nextDataBlockType = DataBlockType::NOTYPE;

    score = 0;

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readFromClient()));
}

void ServerThread::readFromClient(){
    qDebug() << "readFromClient()";

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

        switch (nextDataBlockType){
            case DataBlockType::QSTRING_PSEUDO : {
                QByteArray blockReceived;
                QDataStream blockReceivedStream(&blockReceived, QIODevice::ReadWrite);
                QString pseudo;
                *socketStream >> blockReceived;
                blockReceivedStream >> pseudo;

                this->pseudo = pseudo;

                ServerThread* serverThread;
                foreach(serverThread, *serverThreads){
                    serverThread->sendNewPlayerInfo(pseudo);
                }
            break;
            }

            case DataBlockType::QSTRING_CHAT : {
                QByteArray blockReceived;
                QDataStream blockReceivedStream(&blockReceived, QIODevice::ReadWrite);
                QString msg;
                *socketStream >> blockReceived;
                blockReceivedStream >> msg;

                ServerThread* serverThread;
                foreach(serverThread, *serverThreads){
                    serverThread->sendMsgChat(msg);
                }
            break;
            }

            case DataBlockType::QSTRING_ANSWER : {
                QByteArray blockReceived;
                QDataStream blockReceivedStream(&blockReceived, QIODevice::ReadWrite);
                QString msg;
                *socketStream >> blockReceived;
                blockReceivedStream >> msg;

                ServerThread* serverThread;
                foreach(serverThread, *serverThreads){
                    serverThread->sendMsgAnswer(msg);
                }

            break;
            }
        }

        nextSizeToRead = 0;
        nextDataBlockType = DataBlockType::NOTYPE;
    }
}

//
void ServerThread::sendNewPlayerInfo(QString msg){

}

//
void ServerThread::sendMsgChat(QString msg){
    qDebug() << "sendMsgChat()";

    // Send to the client a new message of the chat
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << msg;
    *socketStream << quint32(blockToSend.size()) << DataBlockType::QSTRING_CHAT << blockToSend;
}

//
void ServerThread::sendMsgAnswer(QString msg){
    qDebug() << "sendMsgAnswer()";

    // Send to the client a new message of the chat
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << msg;
    (*socketStream) << quint32(blockToSend.size()) << DataBlockType::QSTRING_ANSWER << blockToSend;
}
