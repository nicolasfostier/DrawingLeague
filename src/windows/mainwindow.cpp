#include "include/windows/mainwindow.h"



// Constructor
MainWindow::MainWindow() : QMainWindow()
{
    // Initialization of the settings variable
    settings = new QSettings(this);

    //
    mpShortSound = new QMediaPlayer(this);
    mpShortSound->setVolume(50);

    //
    mpTicTac = new QMediaPlayer(this);
    mpTicTac->setVolume(50);
    mpTicTac->setMedia(QUrl("qrc:/sound/after_first_answer.mp3"));

    //
    server = NULL;

    //
    socket = NULL;

    // Room menu
    menuRoom = menuBar()->addMenu(tr("Room"));
        actionJoin = menuRoom->addAction(QIcon(":/images/menubar/join.ico"), tr("Join a room"));
        QObject::connect(actionJoin, SIGNAL(triggered(bool)), this, SLOT(joinRoom()));

        actionCreate = menuRoom->addAction(QIcon(":/images/menubar/create.ico"), tr("Create a room"));
        QObject::connect(actionCreate, SIGNAL(triggered(bool)), this, SLOT(createRoom()));

        actionLeave = menuRoom->addAction(QIcon(":/images/menubar/leave.ico"), tr("Leave the room"));
        actionLeave->setVisible(false);
        QObject::connect(actionLeave, SIGNAL(triggered(bool)), this, SLOT(leaveRoom()));

        actionQuit = menuRoom->addAction(QIcon(":/images/menubar/quit.ico"), tr("Quit"));
        QObject::connect(actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));

    // Tools menu
    menuTools = menuBar()->addMenu(tr("Tools"));
        actionSettings = menuTools->addAction(QIcon(":/images/menubar/settings.ico"), tr("Settings"));

    // Help menu
    menuHelp = menuBar()->addMenu(tr("?"));
        actionAbout = menuHelp->addAction(QIcon(":/images/menubar/about.ico"), tr("About"));
        QObject::connect(actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));


    // The main widget which contains everything else
    mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);

        // Its layout
        mainLayout = new QGridLayout(mainWidget);
        mainLayout->setRowStretch(0, 0);
        mainLayout->setRowStretch(1, 1);
        mainLayout->setColumnStretch(0, 0);
        mainLayout->setColumnStretch(1, 0);
        mainLayout->setColumnStretch(2, 1);
        mainLayout->setSpacing(8);

            // Display all the info about the current game
            roomInfo = new RoomInfo(Room(), this);
            mainLayout->addWidget(roomInfo, 0, 0, 1, 3);

            // Toolbar which contains all the tool to draw on the canvas for the artist
            drawingToolsBar = new QToolBar(mainWidget);
            drawingToolsBar->setOrientation(Qt::Vertical);
            drawingToolsBar->setStyleSheet("QToolBar{spacing:2px;}");
            mainLayout->addWidget(drawingToolsBar, 1, 0, 1, 1);

                // Represent the pen
                penLabel = new QLabel(this);
                penLabel->setStyleSheet("background: white; border: 1px solid grey;");
                penLabelPixmap = new QPixmap(36,36);
                penLabelPixmap->fill(Qt::transparent);
                penLabelBrush.setColor(Qt::black);
                penLabelBrush.setStyle(Qt::SolidPattern);
                QPen penPen;
                penPen.setColor(QColor(50,50,50));
                penPen.setWidth(1);
                penPen.setStyle(Qt::SolidLine);
                penPen.setCapStyle(Qt::RoundCap);
                penPen.setJoinStyle(Qt::RoundJoin);
                penLabelPainter = new QPainter(penLabelPixmap);
                penLabelPainter->setPen(penPen);
                penLabelPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
                drawingToolsBar->addWidget(penLabel);
                drawingToolsBar->addSeparator();

                // Regroup the drawing tool the artist can use
                drawingToolsActionGroup = new QActionGroup(drawingToolsBar);
                QObject::connect(drawingToolsActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateDrawingTools()));
                QObject::connect(drawingToolsActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(sendDrawingToolType()));

                    // Pen
                    penAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/pen.ico"), tr("Pen"));
                    penAction->setCheckable(true);
                    penAction->setChecked(true);

                    // Eraser
                    eraserAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/eraser.ico"), tr("Eraser"));
                    eraserAction->setCheckable(true);

                drawingToolsBar->addActions(drawingToolsActionGroup->actions());
                drawingToolsBar->addSeparator();

                // Spinbox
                penWidthSpinBox = new QSpinBox(this);
                penWidthSpinBox->setRange(2,30);
                penWidthSpinBox->setValue(4);
                penWidthSpinBox->setToolTip(tr("Width of the pen"));
                penWidthSpinBox->setStyleSheet("margin: 0 0 2px 0;");
                drawingToolsBar->addWidget(penWidthSpinBox);

                // Slider
                penWidthSlider = new QSlider(Qt::Vertical, drawingToolsBar);
                penWidthSlider->setToolTip(tr("Width of the pen"));
                penWidthSlider->setRange(2,30);
                penWidthSlider->setValue(4);
                penWidthSlider->setMaximumHeight(50);
                QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDrawingTools()));
                QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), penWidthSpinBox, SLOT(setValue(int)));
                QObject::connect(penWidthSpinBox, SIGNAL(valueChanged(int)), penWidthSlider, SLOT(setValue(int)));
                drawingToolsBar->addWidget(penWidthSlider);

                // Pick the color of the pen
                actionColor = drawingToolsBar->addAction(QIcon(":/images/drawingtools/color.ico"), tr("Pick the color of the pen"));
                QObject::connect(actionColor, SIGNAL(triggered(bool)), this, SLOT(changeColor()));
                    selectedColor = Qt::black;
                drawingToolsBar->addSeparator();


                // Reset
                actionReset = drawingToolsBar->addAction(QIcon(":/images/drawingtools/reset.ico"), tr("Reset the canvas"));
                actionReset->setCheckable(false);

            QLayout* layoutDrawingToolsBar = drawingToolsBar->layout();
            for(int i = 0; i < layoutDrawingToolsBar->count(); i++){
                if(i != 1 && i != 4 && i != 8){
                    layoutDrawingToolsBar->itemAt(i)->setAlignment(Qt::AlignCenter);
                }
            }

            // The canvas, where the artist can draw
            canvasLabel = new Canvas(PEN, Qt::red, penWidthSlider->value(), 600,600, this);
            canvasLabel->setFixedSize(canvasLabel->sizeHint());
            QObject::connect(actionReset, SIGNAL(triggered(bool)), this, SLOT(resetCanvas()));
            this->updateDrawingTools();

            mainLayout->addWidget(canvasLabel, 1, 1, 1, 1);

            // Splitter which connects the chat with the players and the answers
            answersPlayersChatSplitter = new QSplitter(Qt::Vertical, mainWidget);
            mainLayout->addWidget(answersPlayersChatSplitter, 1, 2, 1, 1);

                // Splitter witch connects the players with the answers
                answersPlayersSplitter = new QSplitter(Qt::Horizontal, answersPlayersChatSplitter);

                     // Answers
                    answersWidget = new QWidget(answersPlayersSplitter);

                        answersLayout = new QVBoxLayout(answersWidget);
                        answersLayout->setContentsMargins(0,0,0,0);

                            // Title
                            answersTitle = new QLabel("<center><b>" + tr("Answers :") + "</b></center>", answersWidget);
                            answersLayout->addWidget(answersTitle);

                            // All the answers
                            answersTextEdit = new QTextEdit(answersWidget);
                            answersTextEdit->setReadOnly(true);
                            answersLayout->addWidget(answersTextEdit);

                            // Where players can enter their answers
                            answerLineEdit = new QLineEdit(answersWidget);
                            QObject::connect(answerLineEdit, SIGNAL(returnPressed()), this, SLOT(sendAnswer()));
                            answersLayout->addWidget(answerLineEdit);

                    // Players
                    playersWidget = new QWidget(answersPlayersSplitter);

                        playersLayout = new QVBoxLayout(playersWidget);
                        playersLayout->setContentsMargins(0,0,0,0);

                            // Title
                            playersTitle = new QLabel("<center><b>" + tr("Players :") + "</b></center>", playersWidget);
                            playersLayout->addWidget(playersTitle);

                            // List of all players connected to the room
                            playersTable = new QTableWidget(answersPlayersSplitter);
                            playersTable->setColumnCount(2);
                            QStringList horizontalHeaderLabels;
                            horizontalHeaderLabels.append(tr("Pseudo"));
                            horizontalHeaderLabels.append(tr("Score"));
                            playersTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
                            playersTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
                            playersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
                            playersTable->verticalHeader()->hide();
                            playersLayout->addWidget(playersTable);

                // Customize the handle
                answersPlayersSplitter->setCollapsible(0, false);
                answersPlayersSplitter->setCollapsible(1, false);
                answersPlayersSplitter->setHandleWidth(8);
                QSplitterHandle *handleSplitter = answersPlayersSplitter->handle(1);
                QHBoxLayout *layoutHandleSplitter = new QHBoxLayout(handleSplitter);
                layoutHandleSplitter->setContentsMargins(0,0,0,0);
                QFrame *lineHandleSplitter = new QFrame(answersPlayersSplitter);
                lineHandleSplitter->setFrameShape(QFrame::VLine);
                lineHandleSplitter->setFrameShadow(QFrame::Raised);
                lineHandleSplitter->setFixedHeight(50);
                layoutHandleSplitter->addWidget(lineHandleSplitter);


                // Chat
                chatWidget = new QWidget(answersPlayersChatSplitter);

                    chatLayout = new QVBoxLayout(chatWidget);
                    chatLayout->setContentsMargins(0,0,0,0);

                        // Title
                        chatTitle = new QLabel("<center><b>" + tr("Chat") + "</b></center>", playersWidget);
                        chatLayout->addWidget(chatTitle);

                        // All the messages sent by players
                        chatTextEdit = new QTextEdit(answersPlayersChatSplitter);
                        chatTextEdit->setReadOnly(true);
                        chatLayout->addWidget(chatTextEdit);

                        // Where players can enter their messages to chat with the others players
                        chatLineEdit = new QLineEdit(answersPlayersChatSplitter);
                        QObject::connect(chatLineEdit, SIGNAL(returnPressed()), this, SLOT(sendChat()));
                        chatLayout->addWidget(chatLineEdit);

            // Customize the handle
            answersPlayersChatSplitter->setCollapsible(0, false);
            answersPlayersChatSplitter->setCollapsible(1, false);
            answersPlayersChatSplitter->setHandleWidth(8);
            handleSplitter = answersPlayersChatSplitter->handle(1);
            layoutHandleSplitter = new QHBoxLayout(handleSplitter);
            layoutHandleSplitter->setContentsMargins(0,0,0,0);
            lineHandleSplitter = new QFrame(answersPlayersChatSplitter);
            lineHandleSplitter->setFrameShape(QFrame::HLine);
            lineHandleSplitter->setFrameShadow(QFrame::Raised);
            lineHandleSplitter->setFixedWidth(200);
            layoutHandleSplitter->addWidget(lineHandleSplitter);

            //
            this->artistMode(true);
}



// Destructor
MainWindow::~MainWindow(){
    if(isHosting()){
        server->deleteLater();
    }
    if(isConnected()){
        socket->deleteLater();
        delete dataBlockReader;
        delete dataBlockWriter;
    }
}


// Methods

//
bool MainWindow::isConnected(){
    return socket != NULL;
}

//
bool MainWindow::isHosting(){
    return server != NULL;
}


//
void MainWindow::toggleJoinCreateLeave(){
    this->actionJoin->setVisible(!this->actionJoin->isVisible());
    this->actionCreate->setVisible(!this->actionCreate->isVisible());
    this->actionLeave->setVisible(!this->actionLeave->isVisible());
}

//
void MainWindow::artistMode(bool isArtist){
    canvasLabel->setIsArtist(isArtist);
    drawingToolsBar->setEnabled(isArtist);
    answerLineEdit->setDisabled(isArtist);
}


// Return the selected draw tool type
DrawingToolType MainWindow::selectedDrawingToolType(){
    if(penAction->isChecked()){
        return PEN;
    }
    else{
        return ERASER;
    }
}

// Refresh the representation of the current pen
void MainWindow::refreshPenLabel(){
    QPoint centre((penLabelPixmap->rect().width() / 2) - 1, (penLabelPixmap->rect().height() / 2) - 1);
    penLabelPixmap->fill(Qt::transparent);

    switch(selectedDrawingToolType()){
        case PEN : {
            penLabelPainter->setBrush(penLabelBrush);
        break;
        }

        case ERASER : {
            penLabelPainter->setBrush(QBrush(Qt::white));
        }
    }

    QRect rect = QRect(0, 0, penWidthSlider->value(), penWidthSlider->value());
    rect.moveCenter(centre);
    penLabelPainter->drawEllipse(rect);
    this->penLabel->setPixmap(*penLabelPixmap);
}


//
void MainWindow::closeEvent(QCloseEvent* event){
    int response = QMessageBox::Yes;
    if(isHosting()){
        response = QMessageBox::warning(this, tr("Are you sure to quit ?"), tr("You are hosting this room. If you decide to quit the game, everybody will be disconnected. "
                                                                               "Quit the game anyway ?"), QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
    }
    else if(isConnected()){
        response = QMessageBox::warning(this, tr("Close the room"), tr("You will be disconnected from the room. Do you really want to quit ? "
                                                                       "Quit the game anyway ?"), QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
    }

    if(response == QMessageBox::Yes){
        event->accept();
    }
    else{
        event->ignore();
    }
}



// Qt slots

// Open a window to enter the information of the room the player want to join
void MainWindow::joinRoom(){
    //
    JoinRoomWindow* jrw = new JoinRoomWindow();
    QObject::connect(jrw, SIGNAL(roomJoined(QTcpSocket*,QString)), this, SLOT(roomJoined(QTcpSocket*,QString)));
}
    //
    void MainWindow::roomJoined(QTcpSocket *socket, QString pseudo){
        //
        this->socket = socket;
        QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(resetInterface()));
        if(!isHosting()){
            QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(serverClosed()));
        }

        //
        this->pseudo = pseudo;

        //
        this->dataBlockReader = new DataBlockReader(socket);

        QObject::connect(dataBlockReader, SIGNAL(roomReceived(Room)), this, SLOT(setRoomInfo(Room)));

        QObject::connect(dataBlockReader, SIGNAL(playerEnteringReceived(Player)), this, SLOT(addPlayer(Player)));
        QObject::connect(dataBlockReader, SIGNAL(playerLeavingReceived(QString)), this, SLOT(removePlayer(QString)));

        QObject::connect(dataBlockReader, SIGNAL(roundStartingReceived(int,QString,QString,int)), this, SLOT(roundStarting(int,QString,QString,int)));

        QObject::connect(dataBlockReader, SIGNAL(answerFoundReceived(QString,int)), this, SLOT(answerFound(QString,int)));

        QObject::connect(dataBlockReader, SIGNAL(answerReceived(Message)), this, SLOT(addAnswer(Message)));
        QObject::connect(dataBlockReader, SIGNAL(chatReceived(Message)), this, SLOT(addChat(Message)));

        QObject::connect(dataBlockReader, SIGNAL(drawingToolTypeReceived(DrawingToolType)), this, SLOT(changeDrawingToolType(DrawingToolType)));
        QObject::connect(dataBlockReader, SIGNAL(drawingToolColorReceived(QColor)), this, SLOT(changeDrawingToolColor(QColor)));
        QObject::connect(dataBlockReader, SIGNAL(drawingToolWidthReceived(int)), this, SLOT(changeDrawingToolWidth(int)));

        QObject::connect(dataBlockReader, SIGNAL(canvasResetReceived(bool)), this, SLOT(resetCanvas(bool)));

        QObject::connect(dataBlockReader, SIGNAL(canvasMousePressEventReceived(QPoint)), canvasLabel, SLOT(mousePressEventFromServer(QPoint)));
        QObject::connect(dataBlockReader, SIGNAL(canvasMouseMoveEventReceived(QPoint)), canvasLabel, SLOT(mouseMoveEventFromServer(QPoint)));
        QObject::connect(dataBlockReader, SIGNAL(canvasMouseReleaseEventReceived(QPoint)), canvasLabel, SLOT(mouseReleaseEventFromServer(QPoint)));

        QObject::connect(dataBlockReader, SIGNAL(serverMsgTypeReadyReceived()), this, SLOT(showServerMsgTypeReady()));
        QObject::connect(dataBlockReader, SIGNAL(serverMsgReadyNeededReceived(int)), this, SLOT(showServerMsgReadyNeeded(int)));


        //
        this->dataBlockWriter = new DataBlockWriter(socket);
        this->dataBlockWriter->sendPlayerEntering(Player(pseudo));

        QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), dataBlockWriter, SLOT(sendDrawingToolWidth(int)));

        QObject::connect(actionReset, SIGNAL(triggered(bool)), dataBlockWriter, SLOT(sendCanvasReset()));

        QObject::connect(canvasLabel, SIGNAL(mousePressEventToSend(QPoint)), dataBlockWriter, SLOT(sendCanvasMousePressEvent(QPoint)));
        QObject::connect(canvasLabel, SIGNAL(mouseMoveEventToSend(QPoint)), dataBlockWriter, SLOT(sendCanvasMouseMoveEvent(QPoint)));
        QObject::connect(canvasLabel, SIGNAL(mouseReleaseEventToSend(QPoint)), dataBlockWriter, SLOT(sendCanvasMouseReleaseEvent(QPoint)));


        //
        this->toggleJoinCreateLeave();

        //
        this->artistMode(false);
    }

// Open a window to set up a server
void MainWindow::createRoom(){
    //
    CreateRoomWindow* crw = new CreateRoomWindow();
    QObject::connect(crw, SIGNAL(roomCreated(Server*,QTcpSocket*,QString)), this, SLOT(roomCreated(Server*,QTcpSocket*,QString)));
}
    //
    void MainWindow::roomCreated(Server *server, QTcpSocket *socket, QString pseudo){
        //
        this->server = server;

        //
        this->roomJoined(socket, pseudo);
    }

//
void MainWindow::leaveRoom(){
    int response = QMessageBox::Yes;
    if(isHosting()){
        response = QMessageBox::warning(this, tr("Close the room"), tr("You are hosting this room. If you decide to disconnect from the room, everybody will. "
                                                                       "Continue anyway ?"), QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
        if(response == QMessageBox::Yes){
            server->deleteLater();
            server = NULL;
        }
    }
    if(isConnected() && response == QMessageBox::Yes){
        this->toggleJoinCreateLeave();

        delete dataBlockReader;
        delete dataBlockWriter;

        socket->deleteLater();
        socket = NULL;
    }
}

//
void MainWindow::about(){
    QMessageBox::about(this, tr("About"), "<h2><b>Drawing League</b></h2>"
                                          "<p><b>" + tr("Version :") + "</b> " + qApp->applicationVersion() + " (<a href='https://github.com/nicolasfostier/DrawingLeague/releases'>" + tr("Latest releases") + "</a>)<br/>" +
                                          "<b>" + tr("Developped by :") + "</b> <a href='https://github.com/nicolasfostier'>Nicolas Fostier</a><br/>" +
                                          "<b>" + tr("Library used :") + "</b> Qt 5.8.0<br/>" +
                                          "<b>" + tr("Logo :") + "</b> Elodie Fostier<br/>" +
                                          "<b>" + tr("Icons :") + "</b> <a href='http://www.customicondesign.com/'>Custom Icon Design</a></p>");
}


// Update the tools set for the canvas
void MainWindow::updateDrawingTools(){
    this->refreshPenLabel();
    this->canvasLabel->setTools(selectedDrawingToolType(), penWidthSlider->value(), selectedColor, penLabelPixmap);
}

// Open a dialog to change the color of the drawing tools
void MainWindow::changeColor(){
    selectedColor = QColorDialog::getColor(selectedColor, this, tr("Pick a color"));
    this->penLabelBrush.setColor(selectedColor);
    this->updateDrawingTools();

    if(isConnected()){
        dataBlockWriter->sendDrawingToolColor(selectedColor);
    }
}

// Ask if the artist really want to reset the canvas
void MainWindow::resetCanvas(bool askConfirmation){
    int response = QMessageBox::Yes;
    if(askConfirmation){
        response = QMessageBox::warning(this, tr("Reset the canvas"), tr("Do you really want to reset the canvas ?"), QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
    }
    if(response == QMessageBox::Yes){
        this->canvasLabel->reset();
    }
}


//
void MainWindow::resetInterface(){
    this->resetCanvas(false);

    this->answersTextEdit->clear();
    this->answerLineEdit->clear();

    this->chatTextEdit->clear();
    this->chatLineEdit->clear();

    this->setRoomInfo(Room());

    Player* player;
    foreach(player, players){
        delete player;
    }
    players.clear();

    this->playersTable->setRowCount(0);

    this->artistMode(true);
}


//
void MainWindow::setRoomInfo(Room room){
    //
    this->room = room;

    //
    this->roomInfo->updateRoom(room);
}


//
void MainWindow::roundStarting(int round, QString artist, QString word, int pointToWin){
    //
    mpTicTac->stop();

    //
    this->room.setRound(round);
    this->room.setArtist(artist);
    this->room.setWord(word);
    this->room.setPointToWin(pointToWin);

    //
    this->roomInfo->updateRoom(room);

    //
    this->canvasLabel->reset();

    //
    if(artist == pseudo){
        mpShortSound->setMedia(QUrl("qrc:/sound/you_are_the_artist.mp3"));
        artistMode(true);
    }
    else{
        mpShortSound->setMedia(QUrl("qrc:/sound/new_round.mp3"));
        artistMode(false);
    }

    Player* player;
    foreach(player, players){
        if(player->getPseudo() == artist){
            player->colorGreen();
        }
        else{
            player->colorWhite();
        }
    }

    mpShortSound->play();
}


//
void MainWindow::answerFound(QString pseudo, int pointWon){
    Player* player = players.find(pseudo).value();
    player->setScore(player->getScore() + pointWon);
    if(player->getPseudo() == room.getArtist()){
        player->colorGreen();
    }
    else{
        player->colorWhite();
    }

    if(player->getPseudo() == pseudo){
        mpShortSound->setMedia(QUrl("qrc:/sound/answer_found_you.mp3"));
    }
    else{
        mpShortSound->setMedia(QUrl("qrc:/sound/answer_found_other.mp3"));
    }
    mpShortSound->play();

    if(mpTicTac->state() == QMediaPlayer::StoppedState){
        mpTicTac->play();
    }
}


//
void MainWindow::addPlayer(Player player){
   Player* newPlayer = new Player(player);

   players.insert(player.getPseudo(), newPlayer);

   newPlayer->addToTableWidget(this->playersTable);

   mpShortSound->setMedia(QUrl("qrc:/sound/player_entering.mp3"));
   mpShortSound->play();
}

//
void MainWindow::removePlayer(QString pseudo){
    delete players.find(pseudo).value();
    players.remove(pseudo);

    playersTable->sortItems(0, Qt::AscendingOrder);
    playersTable->sortItems(1, Qt::DescendingOrder);
    playersTable->setRowCount(playersTable->rowCount() - 1);

    mpShortSound->setMedia(QUrl("qrc:/sound/player_leaving.mp3"));
    mpShortSound->play();
}


//
void MainWindow::addAnswer(Message msg){
    this->answersTextEdit->append(msg.toString(false));
}

//
void MainWindow::addChat(Message msg){
    this->chatTextEdit->append(msg.toString(true));
}


//
void MainWindow::changeDrawingToolType(DrawingToolType drawingToolType){
    switch (drawingToolType){
        case PEN :
            this->penAction->setChecked(true);
        break;

        case ERASER :
            this->eraserAction->setChecked(true);
        break;
    }

    this->updateDrawingTools();
}

//
void MainWindow::changeDrawingToolColor(QColor color){
    selectedColor = color;
    this->penLabelBrush.setColor(selectedColor);
    this->updateDrawingTools();
}

//
void MainWindow::changeDrawingToolWidth(int width){
    QObject::disconnect(penWidthSlider, SIGNAL(valueChanged(int)), dataBlockWriter, SLOT(sendDrawingToolWidth(int)));
    this->penWidthSlider->setValue(width);
    QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), dataBlockWriter, SLOT(sendDrawingToolWidth(int)));
}


//
void MainWindow::showServerMsgTypeReady(){
    Message msg("<b><span style='color:red'>" + tr("Server") + "</span></b>", tr("Type") + "<b> !ready </b>" + tr("in the chat to be enough to start the game."));
    addChat(msg);
}

//
void MainWindow::showServerMsgReadyNeeded(int howManyMoreReadyNeeded){
    Message msg("<b><span style='color:red'>" + tr("Server") + "</span></b>", QString::number(howManyMoreReadyNeeded) + " " + tr("more ready player(s) are needed to start the game."));
    addChat(msg);
}


//
void MainWindow::sendAnswer(){
    if(!this->answerLineEdit->text().isEmpty()){
        if(isConnected()){
            Message msg(pseudo, this->answerLineEdit->text());
            dataBlockWriter->sendAnswer(msg);
        }

        this->answerLineEdit->clear();
    }
}

//
void MainWindow::sendChat(){
    if(!this->chatLineEdit->text().isEmpty()){
        if(isConnected()){
            Message msg(pseudo, this->chatLineEdit->text());
            dataBlockWriter->sendChat(msg);
        }

        this->chatLineEdit->clear();
    }
}


//
void MainWindow::sendDrawingToolType(){
    if(isConnected()){
        dataBlockWriter->sendDrawingToolType(selectedDrawingToolType());
    }
}


//
void MainWindow::serverClosed(){
    if(isConnected()){
        QMessageBox::warning(this, tr("The server is closed"), tr("The server has been shutdown."));

        this->toggleJoinCreateLeave();

        delete dataBlockReader;
        delete dataBlockWriter;
        socket->deleteLater();
        socket = NULL;
    }
}
