#ifndef CONNECTION_H
#define CONNECTION_H



#include <QMessageBox>
#include <QApplication>


#include "include/socket/socketreader.h"
#include "include/socket/socketwriter.h"
#include "include/socket/blocktype.h"



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

		SocketReader* socketReader;
		SocketWriter* socketWriter;

	// Getter
	public :
		bool getIsConnected();
		QString getPseudo();
		QTcpSocket* getSocket();
		SocketReader* getSocketReader();
		SocketWriter* getSocketWriter();

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
