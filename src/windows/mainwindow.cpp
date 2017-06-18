#include "include/windows/mainwindow.h"



// Constructor
MainWindow::MainWindow() : QMainWindow()
{
    // Initialization of the settings variable
    settings = new QSettings(this);

    //
    mpEnteringLeaving = new QMediaPlayer(this);
    mpEnteringLeaving->setVolume(50);

    //
    mpStartEndSkip = new QMediaPlayer(this);
    mpStartEndSkip->setVolume(50);

    //
    mpAnswer = new QMediaPlayer(this);
    mpAnswer->setVolume(50);

    //
    mpHint = new QMediaPlayer(this);
    mpHint->setVolume(50);
    mpHint->setMedia(QUrl("qrc:/sound/hint.mp3"));

    //
    mpTicTac = new QMediaPlayer(this);
    mpTicTac->setVolume(50);
    mpTicTac->setMedia(QUrl("qrc:/sound/after_first_answer.mp3"));

    //
    server = NULL;

    //
    socket = NULL;

    //
    artist = NULL;

    //
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(1000);


    // Room menu
    menuRoom = menuBar()->addMenu(tr("Room"));
        joinAction = menuRoom->addAction(QIcon(":/images/menubar/join.ico"), tr("Join a room"));
        QObject::connect(joinAction, SIGNAL(triggered(bool)), this, SLOT(joinRoom()));

        createAction = menuRoom->addAction(QIcon(":/images/menubar/create.ico"), tr("Create a room"));
        QObject::connect(createAction, SIGNAL(triggered(bool)), this, SLOT(createRoom()));

        leaveAction = menuRoom->addAction(QIcon(":/images/menubar/leave.ico"), tr("Leave the room"));
        leaveAction->setVisible(false);
        QObject::connect(leaveAction, SIGNAL(triggered(bool)), this, SLOT(leaveRoom()));

        quitAction = menuRoom->addAction(QIcon(":/images/menubar/quit.ico"), tr("Quit"));
        QObject::connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    // Tools menu
//    menuTools = menuBar()->addMenu(tr("Tools"));
//        actionSettings = menuTools->addAction(QIcon(":/images/menubar/settings.ico"), tr("Settings"));

    // Help menu
    menuHelp = menuBar()->addMenu(tr("?"));
        aboutAction = menuHelp->addAction(QIcon(":/images/menubar/about.ico"), tr("About"));
        QObject::connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));


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
                penLabel->setFixedSize(56,56);
                penLabel->setAlignment(Qt::AlignCenter);
                penLabel->setStyleSheet("background: white; border: 1px solid grey;");
                penLabelPixmap = new QPixmap(56,56);
                penLabel->setScaledContents(false);
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

                    // Bucket
                    bucketAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/bucket.ico"), tr("Bucket"));
                    bucketAction->setCheckable(true);

                    // Eraser
                    eraserAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/eraser.ico"), tr("Eraser"));
                    eraserAction->setCheckable(true);

                drawingToolsBar->addActions(drawingToolsActionGroup->actions());
                drawingToolsBar->addSeparator();

                // Spinbox
                penWidthSpinBox = new QSpinBox(this);
                penWidthSpinBox->setRange(2,50);
                penWidthSpinBox->setValue(2);
                penWidthSpinBox->setToolTip(tr("Width"));
                penWidthSpinBox->setStyleSheet("margin: 0 0 2px 0;");
                drawingToolsBar->addWidget(penWidthSpinBox);

                // Slider
                penWidthSlider = new QSlider(Qt::Vertical, drawingToolsBar);
                penWidthSlider->setToolTip(tr("Width"));
                penWidthSlider->setRange(2,50);
                penWidthSlider->setValue(2);
                penWidthSlider->setMinimumHeight(100);
                QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDrawingTools()));
                QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), penWidthSpinBox, SLOT(setValue(int)));
                QObject::connect(penWidthSpinBox, SIGNAL(valueChanged(int)), penWidthSlider, SLOT(setValue(int)));
                drawingToolsBar->addWidget(penWidthSlider);

                // Pick the color of the pen
                colorAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/color.ico"), tr("Pick a color"));
                QObject::connect(colorAction, SIGNAL(triggered(bool)), this, SLOT(changeColor()));
                    selectedColor = Qt::black;
                drawingToolsBar->addSeparator();


                // Reset
                resetAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/reset.ico"), tr("Reset the canvas"));
                resetAction->setCheckable(false);
                drawingToolsBar->addSeparator();

                // Hint
                hintAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/hint.ico"), tr("Give a hint"));
                hintAction->setCheckable(false);
                drawingToolsBar->addSeparator();

                // Skip the word
                skipWordAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/skip.ico"), tr("Skip the word\n(you will lose 1 point)"));
                skipWordAction->setCheckable(false);

            QLayout* layoutDrawingToolsBar = drawingToolsBar->layout();
            for(int i = 0; i < layoutDrawingToolsBar->count(); i++){
                if(i != 1 && i != 5 && i != 9 && i != 11 && i != 13){
                    layoutDrawingToolsBar->itemAt(i)->setAlignment(Qt::AlignCenter);
                }
            }

            // The canvas, where the artist can draw
            canvasLabel = new Canvas(PEN, Qt::red, penWidthSlider->value(), 600,600, this);
            canvasLabel->setFixedSize(canvasLabel->sizeHint());
            QObject::connect(resetAction, SIGNAL(triggered(bool)), this, SLOT(resetCanvas()));
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
                        chatTitle = new QLabel("<center><b>" + tr("Chat :") + "</b></center>", playersWidget);
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
            this->updateArtistMode();
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
bool MainWindow::isArtist(){
    if(room.getArtist() == pseudo){
        return true;
    }
    if(!isConnected()){
        return true;
    }
    return false;
}


//
void MainWindow::toggleJoinCreateLeave(){
    this->joinAction->setVisible(!this->joinAction->isVisible());
    this->createAction->setVisible(!this->createAction->isVisible());
    this->leaveAction->setVisible(!this->leaveAction->isVisible());
}

//
void MainWindow::updateArtistMode(){
    canvasLabel->setIsArtist(isArtist());
    drawingToolsBar->setEnabled(isArtist());
    resetAction->setEnabled(isArtist());
    skipWordAction->setEnabled(isArtist());
    hintAction->setEnabled(isArtist());
    answerLineEdit->setDisabled(isArtist());

    updateDrawingTools();
}


// Return the selected draw tool type
DrawingToolType MainWindow::selectedDrawingToolType(){
    if(penAction->isChecked()){
        return PEN;
    }
    else if(bucketAction->isChecked()){
        return BUCKET;
    }
    else{
        return ERASER;
    }
}

// Refresh the representation of the current pen
void MainWindow::refreshPenLabel(){
    QPoint centre((penLabelPixmap->rect().width() / 2) - 1, (penLabelPixmap->rect().height() / 2) - 1);
    penLabelPixmap->fill(Qt::transparent);

    QRect rect;
    switch(selectedDrawingToolType()){
        case PEN : {
            rect = QRect(0, 0, penWidthSlider->value(), penWidthSlider->value());
            penLabelPainter->setBrush(penLabelBrush);
        break;
        }

        case BUCKET : {
            rect = QRect(0, 0, 100, 100);
            penLabelPainter->setBrush(penLabelBrush);
            break;
        }

        case ERASER : {
            rect = QRect(0, 0, penWidthSlider->value(), penWidthSlider->value());
            penLabelPainter->setBrush(QBrush(Qt::white));
            break;
        }
    }

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
        response = QMessageBox::warning(this, tr("Close the room"), tr("You will be disconnected from the room. Do you really want to quit ? "),
                                        QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
    }

    if(response == QMessageBox::Yes){
        event->accept();
    }
    else{
        event->ignore();
    }
}



// Qt slots

//
void MainWindow::showNewUpdateAvailable(QJsonDocument jsonReply){
    QMessageBox::about(this, QObject::tr("Update available !"),"<p>" + QObject::tr("A new version of Drawing League is available on github.") + "<br/>" +
                                                        "<b>" + QObject::tr("Your version :") + "</b> " + qApp->applicationVersion() + "<br/>" +
                                                        "<b>" + QObject::tr("Latest version available :") + "</b> " + jsonReply.object().value("tag_name").toString() + "</p>" +
                                                        "<h3 style='text-align: center'><a href='" + jsonReply.object().value("html_url").toString() + "'>" + QObject::tr("Download the latest version") + "</a></h3>");
}

// Open a window to enter the information of the room the player want to join
void MainWindow::joinRoom(){
    //
    JoinRoomWindow* jrw = new JoinRoomWindow();
    QObject::connect(jrw, SIGNAL(roomJoined(QTcpSocket*,DataBlockReader*,DataBlockWriter*,QString)), this, SLOT(roomJoined(QTcpSocket*,DataBlockReader*,DataBlockWriter*,QString)));
}
    //
    void MainWindow::roomJoined(QTcpSocket *socket, DataBlockReader* dataBlockReader, DataBlockWriter* dataBlockWriter, QString pseudo){
        //
        this->resetCanvas(false);

        //
        this->socket = socket;
        QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(resetAll()));
        if(!isHosting()){
            QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(serverClosed()));
        }

        //
        this->pseudo = pseudo;

        //
        this->dataBlockReader = dataBlockReader;

        QObject::connect(dataBlockReader, SIGNAL(roomReceived(Room)), this, SLOT(newRoom(Room)));

        QObject::connect(dataBlockReader, SIGNAL(playerEnteringReceived(Player)), this, SLOT(addEnteringPlayer(Player)));
        QObject::connect(dataBlockReader, SIGNAL(playerOnlineReceived(Player)), this, SLOT(addOnlinePlayer(Player)));
        QObject::connect(dataBlockReader, SIGNAL(playerLeavingReceived(QString)), this, SLOT(removePlayer(QString)));

        QObject::connect(dataBlockReader, SIGNAL(gameStartingReceived()), this, SLOT(gameStarting()));
        QObject::connect(dataBlockReader, SIGNAL(roundStartingReceived(int,QString,QString,int)), this, SLOT(roundStarting(int,QString,QString,int)));
        QObject::connect(dataBlockReader, SIGNAL(roundEndingReceived(QString)), this, SLOT(roundEnding(QString)));
        QObject::connect(dataBlockReader, SIGNAL(skipWordReceived()), this, SLOT(skipWord()));
        QObject::connect(dataBlockReader, SIGNAL(gameEndingReceived(QString)), this, SLOT(gameEnding(QString)));

        QObject::connect(dataBlockReader, SIGNAL(answerReceived(Message)), this, SLOT(addAnswer(Message)));
        QObject::connect(dataBlockReader, SIGNAL(chatReceived(Message)), this, SLOT(addChat(Message)));
        QObject::connect(dataBlockReader, SIGNAL(answerFoundReceived(QString,int)), this, SLOT(answerFound(QString,int)));

        QObject::connect(dataBlockReader, SIGNAL(hintReceived(QString)), this, SLOT(displayHint(QString)));

        QObject::connect(dataBlockReader, SIGNAL(drawingToolTypeReceived(DrawingToolType)), this, SLOT(changeDrawingToolType(DrawingToolType)));
        QObject::connect(dataBlockReader, SIGNAL(drawingToolColorReceived(QColor)), this, SLOT(changeDrawingToolColor(QColor)));
        QObject::connect(dataBlockReader, SIGNAL(drawingToolWidthReceived(int)), this, SLOT(changeDrawingToolWidth(int)));

        QObject::connect(dataBlockReader, SIGNAL(canvasResetReceived(bool)), this, SLOT(resetCanvas(bool)));

        QObject::connect(dataBlockReader, SIGNAL(canvasMousePressEventReceived(QPoint)), canvasLabel, SLOT(mousePressEventFromServer(QPoint)));
        QObject::connect(dataBlockReader, SIGNAL(canvasMouseMoveEventReceived(QPoint)), canvasLabel, SLOT(mouseMoveEventFromServer(QPoint)));
        QObject::connect(dataBlockReader, SIGNAL(canvasMouseReleaseEventReceived(QPoint)), canvasLabel, SLOT(mouseReleaseEventFromServer(QPoint)));

        QObject::connect(dataBlockReader, SIGNAL(serverMsgReadyNeededReceived(int)), this, SLOT(showServerMsgReadyNeeded(int)));

        //
        this->dataBlockWriter = dataBlockWriter;

        QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)), dataBlockWriter, SLOT(sendDrawingToolWidth(int)));

        QObject::connect(resetAction, SIGNAL(triggered(bool)), dataBlockWriter, SLOT(sendCanvasReset()));

        QObject::connect(hintAction, SIGNAL(triggered(bool)), dataBlockWriter, SLOT(sendHint()));

        QObject::connect(skipWordAction, SIGNAL(triggered(bool)), dataBlockWriter, SLOT(sendSkipWord()));

        QObject::connect(canvasLabel, SIGNAL(mousePressEventToSend(QPoint)), dataBlockWriter, SLOT(sendCanvasMousePressEvent(QPoint)));
        QObject::connect(canvasLabel, SIGNAL(mouseMoveEventToSend(QPoint)), dataBlockWriter, SLOT(sendCanvasMouseMoveEvent(QPoint)));
        QObject::connect(canvasLabel, SIGNAL(mouseReleaseEventToSend(QPoint)), dataBlockWriter, SLOT(sendCanvasMouseReleaseEvent(QPoint)));

        //
        this->toggleJoinCreateLeave();

        //
        this->updateArtistMode();
    }

// Open a window to set up a server
void MainWindow::createRoom(){
    //
    CreateRoomWindow* crw = new CreateRoomWindow();
    QObject::connect(crw, SIGNAL(roomCreated(Server*,QTcpSocket*,DataBlockReader*,DataBlockWriter*,QString)), this, SLOT(roomCreated(Server*,QTcpSocket*,DataBlockReader*,DataBlockWriter*,QString)));
}
    //
    void MainWindow::roomCreated(Server *server, QTcpSocket *socket, DataBlockReader* dataBlockReader, DataBlockWriter* dataBlockWriter, QString pseudo){
        //
        this->server = server;

        //
        this->roomJoined(socket, dataBlockReader, dataBlockWriter, pseudo);
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
                                          "<b>" + tr("Version :") + "</b> " + qApp->applicationVersion() + " (<a href='https://github.com/nicolasfostier/DrawingLeague/releases'>" + tr("Latest releases") + "</a>)<br/>" +
                                          "<b>" + tr("Developped by :") + "</b> <a href='https://github.com/nicolasfostier'>Nicolas Fostier</a><br/>" +
                                          "<b>" + tr("Library used :") + "</b> Qt 5.8.0<br/>" +
                                          "<b>" + tr("Logo :") + "</b> Elodie Fostier<br/>" +
                                          "<b>" + tr("Icons :") + "</b> <a href='http://www.customicondesign.com/'>Custom Icon Design</a><br/>" +
                                          "<b>" + tr("Sounds :") + "</b>" +
                                          "<ul style='margin-top : 0px;'>" +
                                                "<li><a href='https://www.youtube.com/watch?v=xzLt7nvdtjw'>after_first_answer.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/Bertrof/sounds/131660/'>answer_found_other.mp3 +" + tr("(shortened)") + "</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/Bertrof/sounds/131660/'>answer_found_you.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/Bertrof/sounds/131657/'>artist_has_failed_skipped.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/chripei/sounds/165491/'>end_game.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/HerbertBoland/sounds/33369/'>hint.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/shinephoenixstormcrow/sounds/337049/'>new_round.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/rhodesmas/sounds/322897/'>player_entering.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/rhodesmas/sounds/322895/'>player_leaving.mp3</a></li>" +
                                                "<li><a href='https://www.freesound.org/people/fins/sounds/171671/'>you_are_the_artist.mp3</a></li>" +
                                          "</ul>");
}


// Update the tools set for the canvas
void MainWindow::updateDrawingTools(){
    this->refreshPenLabel();
    if(isArtist()){
        this->canvasLabel->setTools(selectedDrawingToolType(), penWidthSlider->value(), selectedColor, penLabelPixmap);
    }
    else{
        this->canvasLabel->setTools(selectedDrawingToolType(), penWidthSlider->value(), selectedColor, NULL);
    }
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
void MainWindow::resetAll(){
    this->resetCanvas(false);

    this->answersTextEdit->clear();
    this->answerLineEdit->clear();

    this->chatTextEdit->clear();
    this->chatLineEdit->clear();

    QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(oneSecond()));
    timer->stop();
    secondCounter = 0;


    room = Room();
    roomInfo->setRoom(room);

    Player* player;
    foreach(player, players){
        delete player;
    }
    players.clear();

    artist = NULL;

    this->playersTable->setRowCount(0);

    this->updateArtistMode();

    mpEnteringLeaving->stop();
    mpStartEndSkip->stop();
    mpAnswer->stop();
    mpTicTac->stop();
}


//
void MainWindow::newRoom(Room room){
    //
    this->room = room;

    //
    this->roomInfo->setRoom(room);

    //
    if(room.getCurrentRound() != 0){
        Player* player;
        foreach(player, players){
            if(player->getAnswerFound() == true){
                mpTicTac->play();
                break;
            }
            if(player->getIsArtist()){
                artist = player;
            }
        }

        secondCounter = room.getTimeRemaining();
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(oneSecond()));
        oneSecond();
    }
    else{
        answerLineEdit->setDisabled(true);
        canvasLabel->displayTypeReady();
    }
}


//
void MainWindow::oneSecond(){
    roomInfo->setTime(secondCounter, mpTicTac->state() == QMediaPlayer::PlayingState);
    if(secondCounter > 0){
        secondCounter--;
    }
    timer->start();
}

//
void MainWindow::gameStarting(){
    Player* player;
    foreach(player, players){
        player->setScore(0);
    }

    //
    updateDrawingTools();

    //
    resetCanvas(false);
}

//
void MainWindow::roundStarting(int round, QString artist, QString word, int pointToWin){
    //
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(oneSecond()));
    secondCounter = room.getTimeByRound();
    oneSecond();

    //
    this->room.setCurrentRound(round);
    this->room.setArtist(artist);
    this->room.setWord(word);
    this->room.setPointToWin(pointToWin);
    this->roomInfo->setRoom(room);

    //
    this->artist = players.find(artist).value();
    this->artist->setIsArtist(true);
    this->artist->updateColor();
    updateArtistMode();

    //
    if(isArtist()){
        this->word = word;
        mpStartEndSkip->setMedia(QUrl("qrc:/sound/you_are_the_artist.mp3"));
    }
    else{
        mpStartEndSkip->setMedia(QUrl("qrc:/sound/new_round.mp3"));
    }

    //
    playerFoundAnswer = 0;
    hintGiven = 0;

    //
    mpStartEndSkip->play();
}

//
void MainWindow::roundEnding(QString word){
    //
    mpTicTac->stop();

    //
    if(playerFoundAnswer == 0 && room.getPointToWin() != -1){
        //
        if(artist != NULL){
            artist->setScore(artist->getScore() - 1);
        }

        Message msg("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b><i>" + room.getArtist() + "</i> " + tr("has failed to draw the word in time and lose 1 point.") + "</b>");
        addAnswer(msg);

        mpStartEndSkip->setMedia(QUrl("qrc:/sound/artist_has_failed_skipped.mp3"));
        mpStartEndSkip->play();
    }

    //
    if(artist != NULL){
        artist->setIsArtist(false);
    }
    room.setArtist(" ");
    room.setTimeRemaining(0);
    room.setPointToWin(0);
    roomInfo->setRoom(room);

    //
    updateArtistMode();

    //
    this->canvasLabel->reset();

    //
    answerLineEdit->setDisabled(true);

    //
    Player* player;
    foreach(player, players){
        player->setAnswerFound(false);
        player->updateColor();
    }

    //
    QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(oneSecond()));
    timer->stop();
    secondCounter = 0;

    Message msg("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b>" +tr("The word was") + " <i>" + word + "</i>.</b>");
    addAnswer(msg);
}

//
void MainWindow::skipWord(){
    mpStartEndSkip->setMedia(QUrl("qrc:/sound/artist_has_failed_skipped.mp3"));
    mpStartEndSkip->play();

    //
    if(artist != NULL){
        artist->setScore(artist->getScore() - 1);
    }

    Message msg("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b><i>" + room.getArtist() + "</i> " + tr("has skip the word by losing 1 point.") + "</b>");
    addAnswer(msg);

    //
    room.setPointToWin(-1);
}

//
void MainWindow::gameEnding(QString winner){
    //
    room.setCurrentRound(0);
    room.setArtist(" ");
    room.setWord(" ");
    room.setPointToWin(0);
    roomInfo->setRoom(room);

    //
    canvasLabel->displayWinner(winner);

    //
    mpStartEndSkip->setMedia(QUrl("qrc:/sound/end_game.mp3"));
    mpStartEndSkip->play();
}


//
void MainWindow::answerFound(QString pseudo, int pointWon){
    //
    Player* player = players.find(pseudo).value();
    player->setScore(player->getScore() + pointWon);

    //
    playersTable->sortByColumn(0, Qt::AscendingOrder);
    playersTable->sortByColumn(1, Qt::DescendingOrder);

    //
    if((!isArtist() && this->pseudo == pseudo) || (isArtist() && playerFoundAnswer == 0)){
        mpAnswer->setMedia(QUrl("qrc:/sound/answer_found_you.mp3"));
        answerLineEdit->setDisabled(true);
    }
    else{
        mpAnswer->setMedia(QUrl("qrc:/sound/answer_found_other.mp3"));
    }

    //
    if(!player->getIsArtist()){
        //
        playerFoundAnswer++;

        if(room.getPointToWin() > 5){
            room.setPointToWin(room.getPointToWin() - 1);
            roomInfo->setPointToWin(room.getPointToWin());
        }

        player->setAnswerFound(true);
        player->updateColor();

        mpAnswer->play();
    }    

    //
    if(mpTicTac->state() == QMediaPlayer::StoppedState){
        resetAction->setDisabled(true);
        skipWordAction->setDisabled(true);
        hintAction->setDisabled(true);
        secondCounter = room.getTimeAfterFirstGoodAnswer();
        mpTicTac->play();
        oneSecond();
    }
}


//
void MainWindow::displayHint(QString hint){
    room.setWord(hint);
    roomInfo->setWord(hint);

    hintGiven++;
    room.setPointToWin(room.getPointToWin() - 1);
    roomInfo->setPointToWin(room.getPointToWin());

    if(isArtist() && (room.getPointToWin() <= 5 || hintGiven > (word.size() / 2))){
        hintAction->setDisabled(true);
    }

    Message msg = Message("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b>" + tr("A hint has been given. The number of point to win for this round has been decreased by 1.") + "</b>");
    addAnswer(msg);

    mpHint->play();
}


//
void MainWindow::addEnteringPlayer(Player player){
   addOnlinePlayer(player);

   Message msg = Message("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b><i>" + player.getPseudo() + "</i> " + tr("has joined the room.") + "</b>");
   addChat(msg);

   mpEnteringLeaving->setMedia(QUrl("qrc:/sound/player_entering.mp3"));
   mpEnteringLeaving->play();
}

//
void MainWindow::addOnlinePlayer(Player player){
   Player* newPlayer = new Player(player);
   newPlayer->updateColor();

   players.insert(player.getPseudo(), newPlayer);

   newPlayer->addToTableWidget(this->playersTable);
}

//
void MainWindow::removePlayer(QString pseudo){
    Player* removedPlayer = players.find(pseudo).value();
    players.remove(pseudo);
    delete removedPlayer;
    if (removedPlayer == artist){
        artist = NULL;
    }

    playersTable->sortByColumn(0, Qt::AscendingOrder);
    playersTable->sortByColumn(1, Qt::DescendingOrder);
    playersTable->setRowCount(playersTable->rowCount() - 1);

    Message msg = Message("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b><i>" + pseudo + "</i> " + tr("has left the room.") + "</b>");
    addChat(msg);

    mpEnteringLeaving->setMedia(QUrl("qrc:/sound/player_leaving.mp3"));
    mpEnteringLeaving->play();
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

        case BUCKET :
            this->bucketAction->setChecked(true);
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
void MainWindow::showServerMsgReadyNeeded(int howManyMoreReadyNeeded){
    Message msg("<b><span style='color: #de4d4d'>" + tr("Server") + "</span></b>", "<b><i>" + QString::number(howManyMoreReadyNeeded) + " </i>" + tr("more ready player(s) are needed to start the game.") + "</b>");
    addChat(msg);
}


//
void MainWindow::sendAnswer(){
    if(!this->answerLineEdit->text().isEmpty()){
        if(isConnected()){
            Message msg(pseudo.toHtmlEscaped(), this->answerLineEdit->text().toHtmlEscaped());
            dataBlockWriter->sendAnswer(msg);
        }

        this->answerLineEdit->clear();
    }
}

//
void MainWindow::sendChat(){
    if(!this->chatLineEdit->text().isEmpty()){
        if(isConnected()){
            Message msg(pseudo.toHtmlEscaped(), this->chatLineEdit->text().toHtmlEscaped());
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

        resetAll();
    }
}
