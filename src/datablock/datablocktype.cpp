#include "include/datablock/datablocktype.h"



//
QDataStream& operator<<(QDataStream& dataStream, DataBlockType dataBlockType) {
	return dataStream << static_cast<quint16>(dataBlockType);
}

//
QDataStream& operator>>(QDataStream& dataStream, DataBlockType& dataBlockType) {
	quint16 value;
	dataStream >> value;

	if (dataStream.status() == QDataStream::Ok){
		dataBlockType = DataBlockType(value);
	}
	return dataStream;
}
