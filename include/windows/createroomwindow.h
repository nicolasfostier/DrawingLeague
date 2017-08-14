#ifndef CREATEROOMWINDOW_H
#define CREATEROOMWINDOW_H



#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QStandardPaths>


#include "include/server/server.h"
#include "include/datablock/datablockreader.h"
#include "include/datablock/datablockwriter.h"
#include "include/gameinfo/connection.h"



// Window to create a new room
class CreateRoomWindow : public QDialog
{
	Q_OBJECT

	// Variables
	private :
		Server* server;
		Connection* connection;

		QString dictionaryPath;


		QGridLayout* layout;

			QFont labelFont;

			QLabel* pseudoLabel;
			QLineEdit* pseudoLineEdit;

			QLabel* roomNameLabel;
			QLineEdit* roomNameLineEdit;

			QLabel* portLabel;
			QLineEdit* portLineEdit;

			QLabel* maxPlayersLabel;
			QLineEdit* maxPlayersLineEdit;

			QLabel* dictionaryTypeLabel;
			QComboBox* dictionaryTypeComboBox;
			QStackedLayout* dictionaryStackedLayout;
				QComboBox* dictionaryStandardComboBox;
				QPushButton* browseDictionaryButton;

			QLabel* maxRoundLabel;
			QLineEdit* maxRoundLineEdit;

			QLabel* timeByRoundLabel;
			QLineEdit* timeByRoundLineEdit;

			QLabel* timeAfterFirstGoodAnswerLabel;
			QLineEdit* timeAfterFirstGoodAnswerLineEdit;

			QWidget* createOrCancel;
				QHBoxLayout* layoutConnectOrCancel;
					QPushButton* cancelButton;
					QPushButton* createButton;

	// Constructor
	public :
		CreateRoomWindow();

	// Destructor
	public :
		~CreateRoomWindow();

	// Qt slots
	public slots :
		void browseDictionary();

		void createRoom();

		void newConnection();
		void hasEnteredTheGame();

	// Signals
   signals :
		void roomCreated(Server* server, Connection* connection);
};



#endif // CREATEROOMWINDOW_H
