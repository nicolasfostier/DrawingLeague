#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H


#include <QTime>



//
class ChatMessage
{
    // Variables
    private :
        QString pseudo;
        QString message;
        QTime timestamp;

    // Constructor
    public :
        ChatMessage(QString pseudo, QString message, QTime timestamp = QTime());

    // Operators:
    public :
        friend QDataStream& operator<<(QDataStream& dataStream, ChatMessage chatMessage);
        friend QDataStream& operator>>(QDataStream& dataStream, ChatMessage& chatMessage);
};


//
QDataStream& operator<<(QDataStream& dataStream, ChatMessage chatMessage);
//
QDataStream& operator>>(QDataStream& dataStream, ChatMessage& chatMessage);



#endif // CHATMESSAGE_H
