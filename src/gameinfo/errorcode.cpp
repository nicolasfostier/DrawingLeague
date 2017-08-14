#include "include/gameinfo/errorcode.h"



//
QDataStream& operator<<(QDataStream& dataStream, ErrorCode errorCode) {
	return dataStream << static_cast<quint16>(errorCode);
}

//
QDataStream& operator>>(QDataStream& dataStream, ErrorCode& errorCode) {
	quint16 value;
	dataStream >> value;

	if (dataStream.status() == QDataStream::Ok){
		errorCode = ErrorCode(value);
	}
	return dataStream;
}
