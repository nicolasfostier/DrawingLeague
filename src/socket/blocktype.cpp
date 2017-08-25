#include "include/socket/blocktype.h"



QDataStream& operator<<(QDataStream& dataStream, BlockType dataBlockType) {
	return dataStream << static_cast<quint16>(dataBlockType);
}

QDataStream& operator>>(QDataStream& dataStream, BlockType& dataBlockType) {
	quint16 value;
	dataStream >> value;

	if (dataStream.status() == QDataStream::Ok){
		dataBlockType = BlockType(value);
	}
	return dataStream;
}
