#include "include/socket/connection.h"



// Getter

bool Connection::getIsConnected(){
	return isConnected;
}
QString Connection::getPseudo(){
	return pseudo;
}
QTcpSocket* Connection::getSocket(){
	return socket;
}
SocketReader* Connection::getSocketReader(){
	return socketReader;
}
SocketWriter* Connection::getSocketWriter(){
	return socketWriter;
}



// Setter

void Connection::setIsConnected(bool isConnected){
	this->isConnected = isConnected;
}
void Connection::setShowError(bool showError){
	this->showError = showError;
}



// Constructeurs

Connection::Connection(QString ip, QString port, QString pseudo, QObject* parent) : QObject(parent){
	this->ip = ip;
	this->port = port;
	this->pseudo = pseudo;

	this->socket = NULL;
	this->socketReader = NULL;
	this->socketWriter = NULL;

	this->showError = true;
}
Connection::Connection(qintptr socketDescriptor, QObject* parent) : QObject(parent){
	this->socket = new QTcpSocket(this);
	this->socket->setSocketDescriptor(socketDescriptor);

	this->socketReader = new SocketReader(socket, this);
	this->socketWriter = new SocketWriter(socket, this);

	this->showError = true;
}



// Destructeur

Connection::~Connection(){

}



// Methods

void Connection::connectToTheServer(){
	if(pseudo.size() < 3){
		QMessageBox::critical(NULL, tr("Forbidden pseudo"), tr("Your pseudo must be at least 3 characters long."));
	}
	else{
		this->socket = new QTcpSocket(this);
		QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
						 this, SLOT(socketError()));
		QObject::connect(socket, SIGNAL(connected()),
						 this, SLOT(initDataBlock()));

		this->socket->connectToHost(ip, port.toInt());
		if(!socket->waitForConnected(2000)){
			this->socketError();
		}
	}
}



// Qt slots
void Connection::initDataBlock(){
	socketReader = new SocketReader(socket, this);
	socketWriter = new SocketWriter(socket, this);

	QObject::connect(socketReader, SIGNAL(readyToReceive()),
					 this, SLOT(enterTheGame()));
	QObject::connect(socketReader, SIGNAL(gameErrorReceived(ErrorCode)),
					 this, SLOT(gameError(ErrorCode)));
	QObject::connect(socketReader, SIGNAL(hasEnteredTheGame()),
					 this, SIGNAL(hasEnteredTheGame()));
	socketReader->read();
}

void Connection::enterTheGame(){
	socketWriter->sendEnterTheGame(this->pseudo, qApp->applicationVersion());
}

void Connection::socketError(){
	if(showError){
		showError = false;
		QMessageBox::critical(NULL, tr("Connection error"), socket->errorString());
	}
}

void Connection::gameError(ErrorCode errorCode){
	if(showError){
		showError = false;

		switch(errorCode){
			case ErrorCode::PSEUDO_ALREADY_TAKEN :
				QMessageBox::critical(NULL, tr("Pseudo already used"),
									  tr("This pseudo is already taken."));
			break;

			case ErrorCode::GAME_VERSION_DIFFERENT :
				QMessageBox::critical(NULL, tr("Different game version"),
									  tr("Your game version is different than the one used by the server."));
			break;

			case ErrorCode::SERVER_FULL :
				QMessageBox::critical(NULL, tr("Server full"),
									  tr("The server is full."));
			break;
		}
	}
}
