#include "include/windows/createroomwindow.h"



// Constructor
CreateRoomWindow::CreateRoomWindow() : QDialog()
{
    dictionaryPath = QString();

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
        layout->addWidget(pseudoLineEdit, 0, 1, 1, 2);


        //
        dictionaryTypeLabel = new QLabel(tr("Dictionary type :"), this);
        dictionaryTypeLabel->setFont(labelFont);
        layout->addWidget(dictionaryTypeLabel, 1, 0, 1, 1);

        //
        dictionaryTypeComboBox = new QComboBox(this);
        dictionaryTypeComboBox->insertItem(0, tr("Standard"));
        dictionaryTypeComboBox->insertItem(1, tr("Custom"));
        layout->addWidget(dictionaryTypeComboBox, 1, 1, 1, 1);

            //
            dictionaryStackedLayout = new QStackedLayout();
            QObject::connect(dictionaryTypeComboBox, SIGNAL(currentIndexChanged(int)), dictionaryStackedLayout, SLOT(setCurrentIndex(int)));
            layout->addLayout(dictionaryStackedLayout, 1, 2, 1, 1);

                //
                dictionaryStandardComboBox = new QComboBox(this);
//                dictionaryStandardComboBox->insertItem(0, tr("Easy english"));
                dictionaryStandardComboBox->insertItem(1, tr("Easy french"));
                dictionaryStackedLayout->addWidget(dictionaryStandardComboBox);

                browseDictionaryButton = new QPushButton("...", this);
                QObject::connect(browseDictionaryButton, SIGNAL(clicked(bool)), this, SLOT(browseDictionary()));
                dictionaryStackedLayout->addWidget(browseDictionaryButton);


        //
        portLabel = new QLabel(tr("Server port :"), this);
        portLabel->setFont(labelFont);
        layout->addWidget(portLabel, 2, 0, 1, 1);

        //
        portLineEdit = new QLineEdit(this);
        portLineEdit->setText("23232");
        layout->addWidget(portLineEdit, 2, 1, 1, 2);


        //
        roomNameLabel = new QLabel(tr("Room name :"), this);
        roomNameLabel->setFont(labelFont);
        layout->addWidget(roomNameLabel, 3, 0, 1, 1);

        //
        roomNameLineEdit = new QLineEdit(this);
        roomNameLineEdit->setText("Drawing League");
        layout->addWidget(roomNameLineEdit, 3, 1, 1, 2);


        //
        maxRoundLabel = new QLabel(tr("Number of rounds :"), this);
        maxRoundLabel->setFont(labelFont);
        layout->addWidget(maxRoundLabel, 4, 0, 1, 1);

        //
        maxRoundLineEdit = new QLineEdit(this);
        maxRoundLineEdit->setText("10");
        layout->addWidget(maxRoundLineEdit, 4, 1, 1, 2);


        //
        maxPlayersLabel = new QLabel(tr("Maximum number of players :"), this);
        maxPlayersLabel->setFont(labelFont);
        layout->addWidget(maxPlayersLabel, 5, 0, 1, 1);

        //
        maxPlayersLineEdit = new QLineEdit(this);
        maxPlayersLineEdit->setText("10");
        layout->addWidget(maxPlayersLineEdit, 5, 1, 1, 2);


        //
        timeByRoundLabel = new QLabel(tr("Time by round (in second) :"), this);
        timeByRoundLabel->setFont(labelFont);
        layout->addWidget(timeByRoundLabel, 6, 0, 1, 1);

        //
        timeByRoundLineEdit = new QLineEdit(this);
        timeByRoundLineEdit->setText("120");
        layout->addWidget(timeByRoundLineEdit, 6, 1, 1, 2);


        //
        timeAfterFirstGoodAnswerLabel = new QLabel(tr("Time after the first good answer (in second) :"), this);
        timeAfterFirstGoodAnswerLabel->setFont(labelFont);
        layout->addWidget(timeAfterFirstGoodAnswerLabel, 7, 0, 1, 1);

        //
        timeAfterFirstGoodAnswerLineEdit = new QLineEdit(this);
        timeAfterFirstGoodAnswerLineEdit->setText("15");
        layout->addWidget(timeAfterFirstGoodAnswerLineEdit, 7, 1, 1, 2);


        //
        createOrCancel = new QWidget(this);
        layout->addWidget(createOrCancel, 8, 0, 1, 3, Qt::AlignHCenter);

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
void CreateRoomWindow::browseDictionary(){
    QString newDictionaryPath = QFileDialog::getOpenFileName(this, tr("Choose your dictionary"), QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory), tr("Text files") +" (*.txt)");
    if(!newDictionaryPath.isEmpty()){
        dictionaryPath = newDictionaryPath;
        browseDictionaryButton->setText(QString(QFileInfo(dictionaryPath).fileName()));
    }
}


//
void CreateRoomWindow::createRoom(){
    if(pseudoLineEdit->text().size() < 3){
        QMessageBox::critical(this, tr("Forbidden pseudo"), tr("Your pseudo must be at least 3 characters long !"));
    }
    else if(roomNameLineEdit->text().size() < 3){
        QMessageBox::critical(this, tr("Forbidden room name"), tr("The name of the room must be at least 3 characters long !"));
    }
    else if(maxRoundLineEdit->text().toInt() <= 0){
        QMessageBox::critical(this, tr("Forbidden number of rounds"), tr("The maximum number of rounds by game must be strictly positive !"));
    }
    else if(maxPlayersLineEdit->text().toInt() <= 0){
        QMessageBox::critical(this, tr("Forbidden maximum number of players"), tr("The maximum number of players must be strictly positive !"));
    }
    else if(timeByRoundLineEdit->text().toInt() < 10){
        QMessageBox::critical(this, tr("Forbidden value for 'time by round'"), tr("The time by round must be at least 10 seconds !"));
    }
    else if(timeAfterFirstGoodAnswerLineEdit->text().toInt() < 0){
        QMessageBox::critical(this, tr("Forbidden value for 'time after first good answer'"), tr("The time remaining after a first good answer must be positive !"));
    }
    else{
        if(dictionaryTypeComboBox->currentText() == tr("Standard")){
            if(dictionaryStandardComboBox->currentText() == tr("Easy english")){
                dictionaryPath = ":/dictionaries/easy_english.txt";
            }
            else if(dictionaryStandardComboBox->currentText() == tr("Easy french")){
                dictionaryPath = ":/dictionaries/easy_french.txt";
            }
        }

        Room room(roomNameLineEdit->text(),
                  maxRoundLineEdit->text().toInt(),
                  maxPlayersLineEdit->text().toInt(),
                  timeByRoundLineEdit->text().toInt(),
                  timeAfterFirstGoodAnswerLineEdit->text().toInt());
        this->server = new Server(portLineEdit->text().toInt(), room, dictionaryPath);
        QObject::connect(server, SIGNAL(loadDictionarySucceed()), this, SLOT(connection()));
        QObject::connect(server, SIGNAL(loadDictionaryFailed()), this, SLOT(dictionaryError()));
    }
}


//
void CreateRoomWindow::dictionaryError(){
    QMessageBox::critical(this, tr("Creation failed"), tr("Impossible to read the dictionary.\n"));
    server->deleteLater();
}



//
void CreateRoomWindow::connection(){
    //
    this->socket = new QTcpSocket();
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connectOk()));
    this->socket->connectToHost(QHostAddress::LocalHost, portLineEdit->text().toInt());
}

//
void CreateRoomWindow::connectOk(){
    dataBlockReader = new DataBlockReader(socket);
    QObject::connect(dataBlockReader, SIGNAL(readyToReceive()), this, SLOT(sendPseudo()));
    QObject::connect(dataBlockReader, SIGNAL(pseudoOk()), this, SLOT(connectAndPseudoOk()));
    QObject::connect(dataBlockReader, SIGNAL(pseudoAlreadyUsed()), this, SLOT(pseudoAlreadyUsed()));

    dataBlockWriter = new DataBlockWriter(socket);
}

//
void CreateRoomWindow::connectError(){
    QMessageBox::critical(this, tr("Creation failed"), tr("Impossible to reach the server.\n"));
    server->deleteLater();
}


//
void CreateRoomWindow::sendPseudo(){
    dataBlockWriter->sendPlayerEntering(Player(this->pseudoLineEdit->text().toHtmlEscaped()));
}


//
void CreateRoomWindow::connectAndPseudoOk(){
    //
    emit roomCreated(server, socket, dataBlockReader, dataBlockWriter, this->pseudoLineEdit->text().toHtmlEscaped());

    //
    this->close();
}

//
void CreateRoomWindow::pseudoAlreadyUsed(){
    QObject::disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError()));

    //
    dataBlockWriter->sendPseudoAlreadyUsed();

    //
    server->deleteLater();
    socket->deleteLater();
    dataBlockReader->deleteLater();
    dataBlockWriter->deleteLater();

    //
    QMessageBox::critical(this, tr("Pseudo already used"), tr("This pseudo is already taken."));
}
