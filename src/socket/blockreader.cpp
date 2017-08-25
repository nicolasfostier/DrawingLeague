#include "include/socket/blockreader.h"



// Getter
QDataStream& BlockReader::getStream(){
	return stream;
}



// Constructor
BlockReader::BlockReader(QIODevice* io){
	buffer.open(QIODevice::ReadWrite);
	stream.setVersion(QDataStream::Qt_5_9);
	stream.setDevice(&buffer);

	quint64 blockSize;
	readMax(io, sizeof(blockSize));
	buffer.seek(0);
	stream >> blockSize;

	readMax(io, blockSize);
	buffer.seek(sizeof(blockSize));
}


// Methods
void BlockReader::readMax(QIODevice* io, quint64 n){
	while(static_cast<quint64>(buffer.size()) < n) {
		if (!io->bytesAvailable()) {
			io->waitForReadyRead(30000);
		}
		buffer.write(io->read(n - buffer.size()));
	}
}
