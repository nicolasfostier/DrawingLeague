#ifndef ERROR_H
#define ERROR_H



#include <QDataStream>



// Errors that player can sadly met in the game
enum class ErrorCode : quint16 {PSEUDO_ALREADY_TAKEN = 0,
								GAME_VERSION_DIFFERENT = 1,
								SERVER_FULL = 2,
};


Q_DECLARE_METATYPE(ErrorCode)

QDataStream& operator<<(QDataStream& dataStream, ErrorCode errorCode);
QDataStream& operator>>(QDataStream& dataStream, ErrorCode& errorCode);



#endif // ERROR_H
