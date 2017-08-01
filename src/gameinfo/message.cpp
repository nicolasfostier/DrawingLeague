#include "include/gameinfo/message.h"



// Getter
QString Message::getPseudo(){
	return pseudo;
}
QString Message::getMessage(){
	return message;
}



// Constructor
Message::Message(QString pseudo, QString message, QDateTime timestamp)
{
	this->pseudo = pseudo;
	this->message = message;
	this->timestamp = timestamp;
}
Message::Message(const Message& msg){
	this->pseudo = msg.pseudo;
	this->message = msg.message;
	this->timestamp = msg.timestamp;
}



// Destructor
Message::~Message(){

}



// Methods

//
QString Message::toString(bool includeDate){
	QString string;

	if(includeDate){
		string.append(timestamp.toLocalTime().toString("[hh:mm:ss] "));
	}
	string.append("<b>" + this->pseudo + " :</b> ");
	string.append(this->message);

	return string;
}



// Operators
QDataStream& operator<<(QDataStream& dataStream, Message chatMessage){
	dataStream << chatMessage.timestamp;
	dataStream << chatMessage.pseudo;
	dataStream << chatMessage.message;

	return dataStream;
}
QDataStream& operator>>(QDataStream& dataStream, Message& chatMessage){
	dataStream >> chatMessage.timestamp;
	dataStream >> chatMessage.pseudo;
	dataStream >> chatMessage.message;

	return dataStream;
}
