#ifndef JOINROOMWINDOW_H
#define JOINROOMWINDOW_H



#include <QDialog>
#include <QTranslator>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDataStream>
#include <QApplication>


#include "include/datablock/datablockreader.h"
#include "include/datablock/datablockwriter.h"
#include "include/gameinfo/errorcode.h"
#include "include/gameinfo/connection.h"



// Window to join a room
class JoinRoomWindow : public QDialog
{
	Q_OBJECT

	// Variables
	private :
		Connection* connection;

		QGridLayout* layout;

			QFont labelFont;

			QLabel* pseudoLabel;
			QLineEdit* pseudoLineEdit;

			QLabel* ipLabel;
			QLineEdit* ipLineEdit;

			QLabel* portLabel;
			QLineEdit* portLineEdit;

			QWidget* connectOrCancel;
				QHBoxLayout* layoutConnectOrCancel;
					QPushButton* cancelButton;
					QPushButton* connectButton;

	// Constructor
	public :
		JoinRoomWindow();

	// Destructor
	public :
		~JoinRoomWindow();

	// Qt slots
	public slots:
		void newConnection();
		void hasEnteredTheGame();

	// Signals
	signals :
		void roomJoined(Connection* connection);
};



#endif // JOINROOMWINDOW_H
