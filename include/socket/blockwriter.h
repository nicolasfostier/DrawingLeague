#ifndef BLOCKWRITER_H
#define BLOCKWRITER_H



#include <QIODevice>
#include <QBuffer>
#include <QDataStream>


#include "include/socket/blocktype.h"



// Write data block
class BlockWriter{
	// Variables
	private :
		QIODevice *io;
		QBuffer buffer;
		QDataStream stream;

	// Getter
	public :
		QDataStream& getStream();

	// Constructor
	public :
		BlockWriter(QIODevice* io);

	// Destructor
	public :
		~BlockWriter();
};



#endif // BLOCKWRITER_H
