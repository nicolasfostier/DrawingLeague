#ifndef BLOCKREADER_H
#define BLOCKREADER_H



#include <QBuffer>
#include <QDataStream>
#include <QDebug>



// Read data block
class BlockReader{
	// Variables
	private :
		QBuffer buffer;
		QDataStream stream;

	// Getter
	public :
		QDataStream& getStream();

	// Constructor
	public :
		BlockReader(QIODevice* io);

	// Methods
	public :
		void readMax(QIODevice* io, quint64 n);
};



#endif // BLOCKREADER_H
