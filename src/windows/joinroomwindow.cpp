#include "include/windows/joinroomwindow.h"



// Constructor
JoinRoomWindow::JoinRoomWindow() : QDialog()
{
    //
    layout = new QGridLayout(this);
    layout->setVerticalSpacing(10);

        //
        labelFont.setBold(true);

        //
        pseudoLabel = new QLabel(tr("Your pseudo :"), this);
        pseudoLabel->setFont(labelFont);
        layout->addWidget(pseudoLabel, 0, 0, 1, 1);

        //
        pseudoLineEdit = new QLineEdit(this);
        layout->addWidget(pseudoLineEdit, 0, 1, 1, 1);


        //
        ipLabel = new QLabel(tr("Server IP :"), this);
        ipLabel->setFont(labelFont);
        layout->addWidget(ipLabel, 2, 0, 1, 1);

        //
        ipLineEdit = new QLineEdit(this);
        layout->addWidget(ipLineEdit, 2, 1, 1, 1);


        //
        portLabel = new QLabel(tr("Server port :"), this);
        portLabel->setFont(labelFont);
        layout->addWidget(portLabel, 3, 0, 1, 1);

        //
        portLineEdit = new QLineEdit(this);
        portLineEdit->setText("23232");
        layout->addWidget(portLineEdit, 3, 1, 1, 1);


        //
        connectOrCancel = new QWidget(this);
        layout->addWidget(connectOrCancel, 4, 0, 1, 2);

            //
            layoutConnectOrCancel = new QHBoxLayout(connectOrCancel);

                //
                cancelButton = new QPushButton(tr("Cancel"), this);
                QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
                layoutConnectOrCancel->addWidget(cancelButton);

                //
                connectButton = new QPushButton(tr("Connect"), this);
                connectButton->setDefault(true);
                QObject::connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(connectToTheServeur()));
                layoutConnectOrCancel->addWidget(connectButton);


    this->setWindowTitle(tr("Join a room"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->show();
}



// Qt slots

//
void JoinRoomWindow::connectToTheServeur(){
    if(pseudoLineEdit->text().size() < 3)
    {
        QMessageBox::critical(this, tr("Forbidden pseudo"), tr("Your pseudo must be at least 3 characters long."));
    }
    else{
        this->socket = new QTcpSocket();
        QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
        QObject::connect(socket, SIGNAL(connected()), this, SLOT(connectOk()));
        socket->connectToHost(ipLineEdit->text(), portLineEdit->text().toInt());
    }
}


//
void JoinRoomWindow::connectOk(){
    dataBlockReader = new DataBlockReader(socket);
    QObject::connect(dataBlockReader, SIGNAL(readyToReceive()), this, SLOT(sendPseudo()));
    QObject::connect(dataBlockReader, SIGNAL(pseudoOk()), this, SLOT(connectAndPseudoOk()));
    QObject::connect(dataBlockReader, SIGNAL(pseudoAlreadyUsed()), this, SLOT(pseudoAlreadyUsed()));

    dataBlockWriter = new DataBlockWriter(socket);
}

//
void JoinRoomWindow::connectError(){
    QMessageBox::critical(this, tr("Connection failed"), tr("Impossible to rearch the server.\nEnter a new adress."));
}


//
void JoinRoomWindow::sendPseudo(){
    dataBlockWriter->sendPlayerEntering(Player(this->pseudoLineEdit->text().toHtmlEscaped()));
}


//
void JoinRoomWindow::connectAndPseudoOk(){
    //
    emit roomJoined(socket, dataBlockReader, dataBlockWriter, this->pseudoLineEdit->text().toHtmlEscaped());

    //
    this->close();
}

//
void JoinRoomWindow::pseudoAlreadyUsed(){
    QObject::disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));

    //
    dataBlockWriter->sendPseudoAlreadyUsed();

    //
    socket->deleteLater();
    dataBlockReader->deleteLater();
    dataBlockWriter->deleteLater();

    //
    QMessageBox::critical(this, tr("Pseudo already used"), tr("This pseudo is already taken."));
}
