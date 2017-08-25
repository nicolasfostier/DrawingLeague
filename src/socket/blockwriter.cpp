#include "include/socket/blockwriter.h"


// Getter
QDataStream& BlockWriter::getStream(){
	return stream;
}


// Constructor
BlockWriter::BlockWriter(QIODevice* io){
	this->io = io;

	buffer.open(QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_5_9);
	stream.setDevice(&buffer);
	stream << quint64(0);
}



// Destructor
BlockWriter::~BlockWriter(){
	stream.device()->seek(0);
	stream << static_cast<quint64>(buffer.size());
	io->write(buffer.buffer());
}



