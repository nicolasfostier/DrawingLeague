#include "include/server/serverthread.h"



// Getter
QTcpSocket* ServerThread::getSocket(){
	return connection->getSocket();
}

Player* ServerThread::getPlayer(){
	return player;
}
QString ServerThread::getGameVersion(){
	return gameVersion;
}

SocketReader* ServerThread::getSocketReader(){
	return connection->getSocketReader();
}
SocketWriter* ServerThread::getSocketWriter(){
	return connection->getSocketWriter();
}



// Constructeur
ServerThread::ServerThread(qintptr socketDescriptor)
{
	this->player = NULL;
	this->socketDescriptor = socketDescriptor;

	// Move the object to another thread and start its execution
	QThread* threadCFU = new QThread();
	QObject::connect(threadCFU, SIGNAL(finished()),
					 threadCFU, SLOT(deleteLater()));
	this->moveToThread(threadCFU);
	QObject::connect(threadCFU, SIGNAL(started()),
					 this, SLOT(launch()));
}



// Destructor
ServerThread::~ServerThread(){
	if(player != NULL){
		emit playerLeaving(player->getPseudo(), this, this->getPlayer()->getAnswerFound());
		delete player;
	}

	connection->deleteLater();

	this->thread()->quit();
}



// Qt slots

void ServerThread::launch(){
	timeout = new QTimer(this);
	timeout->setSingleShot(true);
	timeout->setInterval(5000);
	QObject::connect(timeout, SIGNAL(timeout()),
					 this, SLOT(deleteLater()));
	timeout->start();

	connection = new Connection(socketDescriptor, this);

	QObject::connect(connection->getSocket(), SIGNAL(disconnected()),
					 this, SLOT(deleteLater()));

	QObject::connect(connection->getSocketReader(), SIGNAL(enterTheGameReceived(Player,QString)),
					 this, SLOT(setupPlayerInfo(Player, QString)));
	QObject::connect(connection->getSocketReader(), SIGNAL(readyToReceive()),
					 this, SIGNAL(readyToReceive()));

	connection->getSocketWriter()->sendReadyToReceive();

	qInfo()	<< connection->getSocket()->peerAddress().toString()
			<< connection->getSocket()->peerPort()
			<< ": New connection attempt";
}


void ServerThread::setupPlayerInfo(Player player, QString gameVersion){
	player.pseudoToHTMLEscaped();
	this->player = new Player(player);
	this->gameVersion = gameVersion;

	emit wantToEnterTheGame();
}

void ServerThread::hasEnteredTheGame(){
	connection->getSocketWriter()->sendHasEnteredTheGame();
	QObject::disconnect(timeout, SIGNAL(timeout()),
						this, SLOT(deleteLater()));
	timeout->stop();
}

void ServerThread::cantEnterTheGame(ErrorCode errorCode){
	connection->getSocketWriter()->sendGameError(errorCode);
}


// Operators
bool ServerThread::operator!=(ServerThread serverThread){
	return this->getSocket() != serverThread.getSocket();
}
