#include "include/windows/createroomwindow.h"



// Constructor
CreateRoomWindow::CreateRoomWindow(QTcpSocket* socket, Server* server) : QDialog()
{
    this->socket = socket;
    this->server = server;
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connectOk()));

    //
    layout = new QGridLayout(this);
    layout->setVerticalSpacing(10);


        //
        pseudoLabel = new QLabel("<b>" + tr("Your pseudo :") + "</b>", this);
        layout->addWidget(pseudoLabel, 0, 0, 1, 1, Qt::AlignLeft);

        //
        pseudoLineEdit = new QLineEdit(this);
        layout->addWidget(pseudoLineEdit, 0, 1, 1, 1, Qt::AlignRight);


        //
        portLabel = new QLabel("<b>" + tr("Server port :") + "</b>", this);
        layout->addWidget(portLabel, 1, 0, 1, 1, Qt::AlignLeft);

        //
        portLineEdit = new QLineEdit(this);
        portLineEdit->setText("23232");
        layout->addWidget(portLineEdit, 1, 1, 1, 1, Qt::AlignRight);


        //
        roomNameLabel = new QLabel("<b>" + tr("Room name :") + "</b>", this);
        layout->addWidget(roomNameLabel, 2, 0, 1, 1, Qt::AlignLeft);

        //
        roomNameLineEdit = new QLineEdit(this);
        layout->addWidget(roomNameLineEdit, 2, 1, 1, 1, Qt::AlignRight);


        //
        maxRoundLabel = new QLabel("<b>" + tr("Number of round :") + "</b>", this);
        layout->addWidget(maxRoundLabel, 3, 0, 1, 1, Qt::AlignLeft);

        //
        maxRoundLineEdit = new QLineEdit(this);
        layout->addWidget(maxRoundLineEdit, 3, 1, 1, 1, Qt::AlignRight);


        //
        maxPlayersLabel = new QLabel("<b>" + tr("Number of players :") + "</b>", this);
        layout->addWidget(maxPlayersLabel, 4, 0, 1, 1, Qt::AlignLeft);

        //
        maxPlayersLineEdit = new QLineEdit(this);
        layout->addWidget(maxPlayersLineEdit, 4, 1, 1, 1, Qt::AlignRight);


        //
        timeByRoundLabel = new QLabel("<b>" + tr("Time by round (in second) :") + "</b>", this);
        layout->addWidget(timeByRoundLabel, 5, 0, 1, 1, Qt::AlignLeft);

        //
        timeByRoundLineEdit = new QLineEdit(this);
        layout->addWidget(timeByRoundLineEdit, 5, 1, 1, 1, Qt::AlignRight);


        //
        timeAfterFirstGoodAnswerLabel = new QLabel("<b>" + tr("Time after the first good answer (in second) :") + "</b>", this);
        layout->addWidget(timeAfterFirstGoodAnswerLabel, 6, 0, 1, 1, Qt::AlignLeft);

        //
        timeAfterFirstGoodAnswerLineEdit = new QLineEdit(this);
        layout->addWidget(timeAfterFirstGoodAnswerLineEdit, 6, 1, 1, 1, Qt::AlignRight);


        //
        createOrCancel = new QWidget(this);
        layout->addWidget(createOrCancel, 7, 0, 1, 2, Qt::AlignHCenter);

            //
            layoutConnectOrCancel = new QHBoxLayout(createOrCancel);

                //
                cancelButton = new QPushButton(tr("Cancel"), this);
                QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
                layoutConnectOrCancel->addWidget(cancelButton);

                //
                createButton = new QPushButton(tr("Create"), this);
                createButton->setDefault(true);
                QObject::connect(createButton, SIGNAL(clicked(bool)), this, SLOT(createRoom()));
                layoutConnectOrCancel->addWidget(createButton);


    this->setWindowTitle(tr("Create a room"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->show();
}



// Qt slots

//
void CreateRoomWindow::createRoom(){
    Room room(roomNameLineEdit->text(),
              maxRoundLineEdit->text().toInt(),
              maxPlayersLineEdit->text().toInt(),
              timeByRoundLineEdit->text().toInt(),
              timeAfterFirstGoodAnswerLineEdit->text().toInt());
    this->server = new Server(room, portLineEdit->text().toInt());

    //
    socket->connectToHost(QHostAddress::LocalHost, portLineEdit->text().toInt());
}


//
void CreateRoomWindow::connectOk(){
    //
    this->accept();

    // Send the pseudo of the player
    QByteArray blockToSend;
    QDataStream blockToSendStream(&blockToSend, QIODevice::ReadWrite);
    blockToSendStream << pseudoLineEdit->text();
    QDataStream socketStream(socket);
    socketStream << quint32(blockToSend.size()) << DataBlockType::QSTRING_PSEUDO << blockToSend;

    qDebug() << "Connexion OK";

    //
    this->close();
}

//
void CreateRoomWindow::connectError(){
    QMessageBox::critical(this, tr("Creation failed"), tr("Impossible to create the server.\nVerify your settings."));
    delete server;
}
