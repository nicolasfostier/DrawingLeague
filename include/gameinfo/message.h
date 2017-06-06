#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H



#include <QObject>
#include <QDateTime>



//
class Message
{
    // Variables
    private :
        QString pseudo;
        QString message;
        QDateTime timestamp;

    // Getter
    public :
        QString getPseudo();
        QString getMessage();

    // Constructor
    public :
        Message(QString pseudo = "Default", QString message = QString(), QDateTime timestamp = QDateTime::currentDateTimeUtc());
        Message(const Message& msg);

    // Destructor
    public :
        ~Message();

    // Methods
    public :
        //
        QString toString(bool includeDate);

    // Operators
    public :
        friend QDataStream& operator<<(QDataStream& dataStream, Message chatMessage);
        friend QDataStream& operator>>(QDataStream& dataStream, Message& chatMessage);
};

Q_DECLARE_METATYPE(Message)

//
QDataStream& operator<<(QDataStream& dataStream, Message chatMessage);
//
QDataStream& operator>>(QDataStream& dataStream, Message& chatMessage);



#endif // CHATMESSAGE_H
