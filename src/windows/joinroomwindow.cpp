#include "include/windows/joinroomwindow.h"



// Constructor
JoinRoomWindow::JoinRoomWindow() : QDialog()
{
	connection = NULL;

	layout = new QGridLayout(this);
	layout->setVerticalSpacing(10);

		labelFont.setBold(true);


		pseudoLabel = new QLabel(tr("Your pseudo :"), this);
		pseudoLabel->setFont(labelFont);
		layout->addWidget(pseudoLabel, 0, 0, 1, 1);

		pseudoLineEdit = new QLineEdit(this);
		layout->addWidget(pseudoLineEdit, 0, 1, 1, 1);


		QFrame* line = new QFrame(this);
		line->setFrameShape(QFrame::HLine);
		layout->addWidget(line, 1, 0, 1, 3);


		ipLabel = new QLabel(tr("Server IP :"), this);
		ipLabel->setFont(labelFont);
		layout->addWidget(ipLabel, 2, 0, 1, 1);

		ipLineEdit = new QLineEdit(this);
		layout->addWidget(ipLineEdit, 2, 1, 1, 1);


		portLabel = new QLabel(tr("Server port :"), this);
		portLabel->setFont(labelFont);
		layout->addWidget(portLabel, 3, 0, 1, 1);

		portLineEdit = new QLineEdit(this);
		portLineEdit->setText("23232");
		layout->addWidget(portLineEdit, 3, 1, 1, 1);


		connectOrCancel = new QWidget(this);
		layout->addWidget(connectOrCancel, 4, 0, 1, 2);

			layoutConnectOrCancel = new QHBoxLayout(connectOrCancel);

				cancelButton = new QPushButton(tr("Cancel"), this);
				QObject::connect(cancelButton, SIGNAL(clicked(bool)),
								 this, SLOT(close()));
				layoutConnectOrCancel->addWidget(cancelButton);

				connectButton = new QPushButton(tr("Connect"), this);
				connectButton->setDefault(true);
				QObject::connect(connectButton, SIGNAL(clicked(bool)),
								 this, SLOT(newConnection()));
				layoutConnectOrCancel->addWidget(connectButton);


	this->setWindowTitle(tr("Join a room"));
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->show();
}



// Destructor
JoinRoomWindow::~JoinRoomWindow(){
	if(connection != NULL && !connection->getIsConnected()){
		connection->deleteLater();
		connection = NULL;
	}
}



// Qt slots

void JoinRoomWindow::newConnection(){
	if(connection != NULL){
		delete connection;
		connection = NULL;
	}

	connection = new Connection(this->ipLineEdit->text(), this->portLineEdit->text(),
								this->pseudoLineEdit->text());
	QObject::connect(connection, SIGNAL(hasEnteredTheGame()),
					 this, SLOT(hasEnteredTheGame()));

	connection->connectToTheServer();
}

void JoinRoomWindow::hasEnteredTheGame(){
	this->connection->setIsConnected(true);

	emit roomJoined(connection);

	this->close();
}
