#include "include/windows/joinroomwindow.h"



// Constructor
JoinRoomWindow::JoinRoomWindow() : QDialog()
{
    this->socket = new QTcpSocket();
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connectOk()));

    //
    layout = new QGridLayout(this);
    layout->setVerticalSpacing(10);

        //
        pseudoLabel = new QLabel("<b>" + tr("Your pseudo :") + "</b>", this);
        layout->addWidget(pseudoLabel, 0, 0, 1, 1);

        //
        pseudoLineEdit = new QLineEdit(this);
        layout->addWidget(pseudoLineEdit, 0, 1, 1, 1);


        //
        ipLabel = new QLabel("<b>" + tr("Server IP :") + "</b>", this);
        layout->addWidget(ipLabel, 2, 0, 1, 1);

        //
        ipLineEdit = new QLineEdit(this);
        layout->addWidget(ipLineEdit, 2, 1, 1, 1);


        //
        portLabel = new QLabel("<b>" + tr("Server port :") + "</b>", this);
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
        socket->connectToHost(ipLineEdit->text(), portLineEdit->text().toInt());
    }
}

//
void JoinRoomWindow::connectOk(){
    emit roomJoined(socket, this->pseudoLineEdit->text().toHtmlEscaped());

    //
    this->close();
}

//
void JoinRoomWindow::connectError(){
    QMessageBox::critical(this, tr("Connection failed"), tr("Impossible to rearch the server.\nEnter a new adress."));
}
