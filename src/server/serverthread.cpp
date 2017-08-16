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

DataBlockReader* ServerThread::getDataBlockReader(){
	return connection->getDBR();
}
DataBlockWriter* ServerThread::getDataBlockWriter(){
	return connection->getDBW();
}



// Constructeur
ServerThread::ServerThread(QTcpSocket* socket)
{
	this->player = NULL;
	this->socketDescriptor = socket->socketDescriptor();

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

	QObject::connect(connection->getDBR(), SIGNAL(enterTheGameReceived(Player,QString)),
					 this, SLOT(setupPlayerInfo(Player, QString)));
	QObject::connect(connection->getDBR(), SIGNAL(readyToReceive()),
					 this, SIGNAL(readyToReceive()));

	connection->getDBW()->sendReadyToReceive();

	thread()->msleep(100);
}


void ServerThread::setupPlayerInfo(Player player, QString gameVersion){
	player.pseudoToHTMLEscaped();
	this->player = new Player(player);
	this->gameVersion = gameVersion;

	emit wantToEnterTheGame();
}

void ServerThread::hasEnteredTheGame(){
	connection->getDBW()->sendHasEnteredTheGame();
	QObject::disconnect(timeout, SIGNAL(timeout()),
						this, SLOT(deleteLater()));
	timeout->stop();
}

void ServerThread::cantEnterTheGame(ErrorCode errorCode){
	connection->getDBW()->sendGameError(errorCode);
}


// Operators
bool ServerThread::operator!=(ServerThread serverThread){
	return this->getSocket() != serverThread.getSocket();
}
