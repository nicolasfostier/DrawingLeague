#ifndef CONNECTION_H
#define CONNECTION_H



#include <QMessageBox>
#include <QApplication>


#include "include/datablock/datablockreader.h"
#include "include/datablock/datablockwriter.h"
#include "include/datablock/datablocktype.h"



// Handle a TCP connection
class Connection : public QObject{
	Q_OBJECT

	// Variable
	private :
		bool isConnected;
		bool showError;

		QString ip;
		QString port;
		QString pseudo;

		QTcpSocket* socket;

		DataBlockReader* dataBlockReader;
		DataBlockWriter* dataBlockWriter;

	// Getter
	public :
		bool getIsConnected();
		QString getPseudo();
		QTcpSocket* getSocket();
		DataBlockReader* getDBR();
		DataBlockWriter* getDBW();

	// Setter
	public :
		void setIsConnected(bool isConnected);
		void setShowError(bool showError);

	// Constructeur
	public :
		Connection(QString ip, QString port, QString pseudo, QObject* parent = NULL);
		Connection(qintptr socketDescriptor, QObject* parent = NULL);

	// Destructeur
	public :
		~Connection();

	// Methods :
	public :
		void connectToTheServer();

	// Qt slots
	public slots :
		void initDataBlock();
		void enterTheGame();

		void socketError();
		void gameError(ErrorCode errorCode);

	// Qt signals
	signals :
		void hasEnteredTheGame();
};



#endif // CONNECTION_H
