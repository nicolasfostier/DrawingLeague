#include "include/windows/joinroomwindow.h"



// Constructor
JoinRoomWindow::JoinRoomWindow(QTcpSocket* socket) : QDialog()
{
    this->socket = socket;
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connectOk()));

    //
    layout = new QGridLayout(this);
    layout->setVerticalSpacing(10);

        //
        ipLabel = new QLabel("<b>" + tr("Server IP :") + "</b>", this);
        layout->addWidget(ipLabel, 0, 0, 1, 1, Qt::AlignLeft);

        //
        ipLineEdit = new QLineEdit(this);
        layout->addWidget(ipLineEdit, 0, 1, 1, 1, Qt::AlignRight);


        //
        portLabel = new QLabel("<b>" + tr("Server port :") + "</b>", this);
        layout->addWidget(portLabel, 1, 0, 1, 1, Qt::AlignLeft);

        //
        portLineEdit = new QLineEdit(this);
        layout->addWidget(portLineEdit, 1, 1, 1, 1, Qt::AlignRight);


        //
        connectOrCancel = new QWidget(this);
        layout->addWidget(connectOrCancel, 2, 0, 1, 2, Qt::AlignHCenter);

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
    socket->connectToHost(ipLabel->text(), portLabel->text().toInt());
}

//
void JoinRoomWindow::connectOk(){
    this->accept();
    this->close();
}

//
void JoinRoomWindow::connectError(){
    QMessageBox::critical(this, tr("Connection failed"), tr("Impossible to rearch the server.\nEnter a new adress."));
}
