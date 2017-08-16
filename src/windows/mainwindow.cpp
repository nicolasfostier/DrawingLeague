#include "include/windows/mainwindow.h"



// Constructor
MainWindow::MainWindow() : QMainWindow()
{
	settings = new QSettings(this);

	mpEnteringLeaving = new QMediaPlayer(this);
	mpEnteringLeaving->setVolume(50);

	mpStartEndSkip = new QMediaPlayer(this);
	mpStartEndSkip->setVolume(50);

	mpAnswer = new QMediaPlayer(this);
	mpAnswer->setVolume(50);

	mpHint = new QMediaPlayer(this);
	mpHint->setVolume(50);
	mpHint->setMedia(QUrl("qrc:/sound/hint.mp3"));

	mpTicTac = new QMediaPlayer(this);
	mpTicTac->setVolume(50);
	mpTicTac->setMedia(QUrl("qrc:/sound/after_first_answer.mp3"));

	server = NULL;
	connection = NULL;

	artist = NULL;

	menuRoom = menuBar()->addMenu(tr("Room"));
		joinAction = menuRoom->addAction(QIcon(":/images/menubar/join.ico"), tr("Join a room"));
		QObject::connect(joinAction, SIGNAL(triggered(bool)),
						 this, SLOT(joinRoom()));

		createAction = menuRoom->addAction(QIcon(":/images/menubar/create.ico"), tr("Create a room"));
		QObject::connect(createAction, SIGNAL(triggered(bool)),
						 this, SLOT(createRoom()));

		leaveAction = menuRoom->addAction(QIcon(":/images/menubar/leave.ico"), tr("Leave the room"));
		leaveAction->setVisible(false);
		QObject::connect(leaveAction, SIGNAL(triggered(bool)),
						 this, SLOT(leaveRoom()));

		quitAction = menuRoom->addAction(QIcon(":/images/menubar/quit.ico"), tr("Quit"));
		QObject::connect(quitAction, SIGNAL(triggered(bool)),
						 this, SLOT(close()));

//    menuTools = menuBar()->addMenu(tr("Tools"));
//        actionSettings = menuTools->addAction(QIcon(":/images/menubar/settings.ico"), tr("Settings"));

	menuHelp = menuBar()->addMenu(tr("?"));
		aboutAction = menuHelp->addAction(QIcon(":/images/menubar/about.ico"), tr("About"));
		QObject::connect(aboutAction, SIGNAL(triggered(bool)),
						 this, SLOT(about()));


	mainWidget = new QWidget(this);
	this->setCentralWidget(mainWidget);

		mainLayout = new QGridLayout(mainWidget);
		mainLayout->setRowStretch(0, 0);
		mainLayout->setRowStretch(1, 1);
		mainLayout->setColumnStretch(0, 0);
		mainLayout->setColumnStretch(1, 1);
		mainLayout->setSpacing(8);

			roomInfo = new RoomInfo(Room(), this);
			mainLayout->addWidget(roomInfo, 0, 0, 1, 2);

			drawingToolsBar = new QToolBar(mainWidget);
			drawingToolsBar->setOrientation(Qt::Vertical);
			drawingToolsBar->setStyleSheet("QToolBar{spacing:2px;}");
			mainLayout->addWidget(drawingToolsBar, 1, 0, 1, 1, Qt::AlignVCenter | Qt::AlignTop);

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

				drawingToolsActionGroup = new QActionGroup(drawingToolsBar);
				QObject::connect(drawingToolsActionGroup, SIGNAL(triggered(QAction*)),
								 this, SLOT(updateDrawingTools()));
				QObject::connect(drawingToolsActionGroup, SIGNAL(triggered(QAction*)),
								 this, SLOT(sendDrawingToolType()));

					penAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/pen.ico"), tr("Pen"));
					penAction->setCheckable(true);
					penAction->setChecked(true);

					bucketAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/bucket.ico"), tr("Bucket"));
					bucketAction->setCheckable(true);

					eraserAction = drawingToolsActionGroup->addAction(QIcon(":/images/drawingtools/eraser.ico"), tr("Eraser"));
					eraserAction->setCheckable(true);

				drawingToolsBar->addActions(drawingToolsActionGroup->actions());
				drawingToolsBar->addSeparator();

				penWidthSpinBox = new QSpinBox(this);
				penWidthSpinBox->setRange(3,50);
				penWidthSpinBox->setValue(3);
				penWidthSpinBox->setToolTip(tr("Width"));
				penWidthSpinBox->setStyleSheet("margin: 0 0 2px 0;");
				drawingToolsBar->addWidget(penWidthSpinBox);

				penWidthSlider = new QSlider(Qt::Vertical, drawingToolsBar);
				penWidthSlider->setToolTip(tr("Width"));
				penWidthSlider->setRange(3,50);
				penWidthSlider->setValue(3);
				penWidthSlider->setMinimumHeight(100);
				QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)),
								 this, SLOT(updateDrawingTools()));
				QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)),
								 penWidthSpinBox, SLOT(setValue(int)));
				QObject::connect(penWidthSpinBox, SIGNAL(valueChanged(int)),
								 penWidthSlider, SLOT(setValue(int)));
				drawingToolsBar->addWidget(penWidthSlider);

				colorAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/color.ico"), tr("Pick a color"));
				QObject::connect(colorAction, SIGNAL(triggered(bool)),
								 this, SLOT(changeColor()));
					selectedColor = Qt::black;
				drawingToolsBar->addSeparator();


				resetAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/reset.ico"), tr("Reset the canvas"));
				resetAction->setCheckable(false);
				drawingToolsBar->addSeparator();

				hintAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/hint.ico"), tr("Give a hint"));
				hintAction->setCheckable(false);
				drawingToolsBar->addSeparator();

				skipWordAction = drawingToolsBar->addAction(QIcon(":/images/drawingtools/skip.ico"), tr("Skip the word\n(you will lose 1 point)"));
				skipWordAction->setCheckable(false);

			QLayout* layoutDrawingToolsBar = drawingToolsBar->layout();
			int nbTools = layoutDrawingToolsBar->count();
			for(int i = 0; i < nbTools; ++i){
				if(i != 1 && i != 5 && i != 9 && i != 11 && i != 13){
					layoutDrawingToolsBar->itemAt(i)->setAlignment(Qt::AlignCenter);
				}
			}

			mainBlockSplitter = new QSplitter(Qt::Horizontal, mainWidget);
			mainLayout->addWidget(mainBlockSplitter, 1, 1, 1, 1);

				canvasView = new CanvasView(QRectF(0,0,1000,1000), mainBlockSplitter);
				canvasView->setSceneRect(0,0,1000,1000);
				QObject::connect(resetAction, SIGNAL(triggered(bool)),
								 this, SLOT(resetCanvas()));
				this->updateDrawingTools();

				answersPlayersChatSplitter = new QSplitter(Qt::Vertical, mainBlockSplitter);

					answersPlayersSplitter = new QSplitter(Qt::Horizontal, answersPlayersChatSplitter);

						answersWidget = new QWidget(answersPlayersSplitter);

							answersLayout = new QVBoxLayout(answersWidget);
							answersLayout->setContentsMargins(0,0,0,0);

								answersTitle = new QLabel("<center><b>" + tr("Answers :") + "</b></center>", answersWidget);
								answersLayout->addWidget(answersTitle);

								answersTextEdit = new QTextEdit(answersWidget);
								answersTextEdit->setReadOnly(true);
								answersLayout->addWidget(answersTextEdit);

								answerLineEdit = new QLineEdit(answersWidget);
								QObject::connect(answerLineEdit, SIGNAL(returnPressed()),
												 this, SLOT(sendAnswer()));
								answersLayout->addWidget(answerLineEdit);

						playersWidget = new QWidget(answersPlayersSplitter);

							playersLayout = new QVBoxLayout(playersWidget);
							playersLayout->setContentsMargins(0,0,0,0);

								playersTitle = new QLabel("<center><b>" + tr("Players :") + "</b></center>", playersWidget);
								playersLayout->addWidget(playersTitle);

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

					answersPlayersSplitter->setCollapsible(0, false);
					answersPlayersSplitter->setCollapsible(1, false);
					answersPlayersSplitter->setHandleWidth(10);


					chatWidget = new QWidget(answersPlayersChatSplitter);

						chatLayout = new QVBoxLayout(chatWidget);
						chatLayout->setContentsMargins(0,0,0,0);

							chatTitle = new QLabel("<center><b>" + tr("Chat :") + "</b></center>", playersWidget);
							chatLayout->addWidget(chatTitle);

							chatTextEdit = new QTextEdit(chatWidget);
							chatTextEdit->setReadOnly(true);
							chatLayout->addWidget(chatTextEdit);

							chatLineEdit = new QLineEdit(chatWidget);
							QObject::connect(chatLineEdit, SIGNAL(returnPressed()),
											 this, SLOT(sendChat()));
							chatLayout->addWidget(chatLineEdit);

				answersPlayersChatSplitter->setCollapsible(0, false);
				answersPlayersChatSplitter->setCollapsible(1, false);
				answersPlayersChatSplitter->setHandleWidth(2);

			mainBlockSplitter->setCollapsible(0, false);
			mainBlockSplitter->setCollapsible(1, false);
			mainBlockSplitter->setHandleWidth(10);

	this->setStyleSheet(
		"QSplitter::handle:horizontal {"
			"margin-left: 4px;"
			"margin-right: 4px;"
			"background: qlineargradient(x1:0, y1:0, x2:0.75, y2:0, stop:0 #808080, stop:1 #ffffff);"
		"}"
		"QSplitter::handle:vertical {"
			"margin-top: 6px;"
			"margin-bottom: 4px;"
			"background: qlineargradient(x1:0, y1:0, x2:0, y2:0.75, stop:0 #808080, stop:1 #ffffff);"
		"}"
	);

	this->updateArtistMode();
	this->resetAll();

	this->show();

	drawingToolsBar->setFixedSize(drawingToolsBar->sizeHint());

	canvasView->fit();
}



// Destructor
MainWindow::~MainWindow(){
	if(isHosting()){
		server->deleteLater();
	}
	if(this->isConnected()){
		delete connection;
		connection = NULL;
	}
}


// Methods

bool MainWindow::isConnected(){
	return connection != NULL && connection->getIsConnected();
}

bool MainWindow::isHosting(){
	return server != NULL;
}


bool MainWindow::isArtist(){
	if(connection != NULL && roomInfo->getArtist() == connection->getPseudo()){
		return true;
	}
	if(!this->isConnected()){
		return true;
	}
	return false;
}


void MainWindow::toggleJoinCreateLeave(){
	this->joinAction->setVisible(!this->joinAction->isVisible());
	this->createAction->setVisible(!this->createAction->isVisible());
	this->leaveAction->setVisible(!this->leaveAction->isVisible());
}

void MainWindow::updateArtistMode(){
	canvasView->getCanvas()->setIsArtist(isArtist());
	drawingToolsBar->setEnabled(isArtist());
	resetAction->setEnabled(isArtist());
	skipWordAction->setEnabled(isArtist());
	hintAction->setEnabled(isArtist());
	answerLineEdit->setDisabled(isArtist());

	updateDrawingTools();
}


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

void MainWindow::refreshPenLabel(){
	QPoint centre((penLabelPixmap->rect().width() / 2) - 1,
				  (penLabelPixmap->rect().height() / 2) - 1);
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


void MainWindow::closeEvent(QCloseEvent* event){
	int response = QMessageBox::Yes;
	if(isHosting()){
		response = QMessageBox::warning(this, tr("Are you sure to quit ?"),
										tr("You are hosting this room. If you decide to quit the game, everybody will be disconnected. "
																			   "Quit the game anyway ?"),
										QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
	}
	else if(this->isConnected()){
		response = QMessageBox::warning(this, tr("Close the room"),
										tr("You will be disconnected from the room. Do you really want to quit ?"),
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

void MainWindow::showNewUpdateAvailable(QJsonDocument jsonReply){
	QMessageBox::about(this, QObject::tr("Update available !"),
					   "<p>" + QObject::tr("A new version of Drawing League is available on github.") + "<br/>" +
					   "<b>" + QObject::tr("Your version :") + "</b> " + qApp->applicationVersion() + "<br/>" +
					   "<b>" + QObject::tr("Latest version available :") + "</b> " + jsonReply.object().value("tag_name").toString() + "</p>" +
					   "<h3 style='text-align: center'><a href='" + jsonReply.object().value("html_url").toString() + "'>" + QObject::tr("Download the latest version") + "</a></h3>");
}

void MainWindow::joinRoom(){
	JoinRoomWindow* jrw = new JoinRoomWindow();
	QObject::connect(jrw, SIGNAL(roomJoined(Connection*)),
					 this, SLOT(roomJoined(Connection*)));
}
	void MainWindow::roomJoined(Connection* connection){
		this->connection = connection;

		QTcpSocket* socket = connection->getSocket();
		QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(resetAll()));



		DataBlockReader* dataBlockReader = connection->getDBR();

		QObject::connect(dataBlockReader, SIGNAL(roomReceived(Room)),
						 this, SLOT(newRoom(Room)));

		QObject::connect(dataBlockReader, SIGNAL(playerEnteringReceived(QString)),
						 this, SLOT(addEnteringPlayer(QString)));
		QObject::connect(dataBlockReader, SIGNAL(playerOnlineReceived(Player)),
						 this, SLOT(addOnlinePlayer(Player)));
		QObject::connect(dataBlockReader, SIGNAL(playerLeavingReceived(QString)),
						 this, SLOT(removePlayer(QString)));

		QObject::connect(dataBlockReader, SIGNAL(gameStartingReceived()),
						 this, SLOT(gameStarting()));
		QObject::connect(dataBlockReader, SIGNAL(roundStartingReceived(int,QString,QString,int)),
						 this, SLOT(roundStarting(int,QString,QString,int)));
		QObject::connect(dataBlockReader, SIGNAL(roundEndingReceived(QString)),
						 this, SLOT(roundEnding(QString)));
		QObject::connect(dataBlockReader, SIGNAL(gameEndingReceived(QString)),
						 this, SLOT(gameEnding(QString)));

		QObject::connect(dataBlockReader, SIGNAL(answerReceived(Message)),
						 this, SLOT(addAnswer(Message)));
		QObject::connect(dataBlockReader, SIGNAL(chatReceived(Message)),
						 this, SLOT(addChat(Message)));
		QObject::connect(dataBlockReader, SIGNAL(answerFoundReceived(QString,int)),
						 this, SLOT(answerFound(QString,int)));
		QObject::connect(dataBlockReader, SIGNAL(answerCloseReceived()),
						 this, SLOT(answerClose()));

		QObject::connect(dataBlockReader, SIGNAL(hintReceived(QString)),
						 this, SLOT(displayHint(QString)));
		QObject::connect(dataBlockReader, SIGNAL(skipWordReceived()),
						 this, SLOT(skipWord()));

		QObject::connect(dataBlockReader, SIGNAL(drawingToolTypeReceived(DrawingToolType)),
						 this, SLOT(changeDrawingToolType(DrawingToolType)));
		QObject::connect(dataBlockReader, SIGNAL(drawingToolColorReceived(QColor)),
						 this, SLOT(changeDrawingToolColor(QColor)));
		QObject::connect(dataBlockReader, SIGNAL(drawingToolWidthReceived(int)),
						 this, SLOT(changeDrawingToolWidth(int)));

		QObject::connect(dataBlockReader, SIGNAL(canvasResetReceived()),
						 this, SLOT(resetCanvas()));

		QObject::connect(dataBlockReader, SIGNAL(canvasMousePressEventReceived(QPoint)),
						 canvasView->getCanvas(), SLOT(mousePressEventFromServer(QPoint)));
		QObject::connect(dataBlockReader, SIGNAL(canvasMouseMoveEventReceived(QPoint)),
						 canvasView->getCanvas(), SLOT(mouseMoveEventFromServer(QPoint)));
		QObject::connect(dataBlockReader, SIGNAL(canvasMouseReleaseEventReceived(QPoint)),
						 canvasView->getCanvas(), SLOT(mouseReleaseEventFromServer(QPoint)));

		QObject::connect(dataBlockReader, SIGNAL(readyNeededReceived(int)),
						 canvasView, SLOT(updateReadyNeeded(int)));



		DataBlockWriter* dataBlockWriter = connection->getDBW();

		QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)),
						 dataBlockWriter, SLOT(sendDrawingToolWidth(int)));
		QObject::connect(resetAction, SIGNAL(triggered(bool)),
						 dataBlockWriter, SLOT(sendCanvasReset()));
		QObject::connect(hintAction, SIGNAL(triggered(bool)),
						 dataBlockWriter, SLOT(sendHint()));
		QObject::connect(skipWordAction, SIGNAL(triggered(bool)),
						 dataBlockWriter, SLOT(sendSkipWord()));

		QObject::connect(canvasView->getReadyButton(), SIGNAL(ready()),
						 dataBlockWriter, SLOT(sendReady()));

		QObject::connect(canvasView->getCanvas(), SIGNAL(mousePressEventToSend(QPoint)),
						 dataBlockWriter, SLOT(sendCanvasMousePressEvent(QPoint)));
		QObject::connect(canvasView->getCanvas(), SIGNAL(mouseMoveEventToSend(QPoint)),
						 dataBlockWriter, SLOT(sendCanvasMouseMoveEvent(QPoint)));
		QObject::connect(canvasView->getCanvas(), SIGNAL(mouseReleaseEventToSend(QPoint)),
						 dataBlockWriter, SLOT(sendCanvasMouseReleaseEvent(QPoint)));

		this->toggleJoinCreateLeave();

		this->updateArtistMode();

		this->resetCanvas(false);

		this->connection->getDBW()->sendReadyToReceive();
	}

void MainWindow::createRoom(){
	CreateRoomWindow* crw = new CreateRoomWindow();
	QObject::connect(crw, SIGNAL(roomCreated(Server*, Connection*)),
					 this, SLOT(roomCreated(Server*, Connection*)));
}
	void MainWindow::roomCreated(Server *server, Connection* connection){
		this->server = server;

		this->roomJoined(connection);
	}

void MainWindow::leaveRoom(){
	int response = QMessageBox::Yes;
	if(isHosting()){
		response = QMessageBox::warning(this, tr("Close the room"),
										tr("You are hosting this room. If you decide to disconnect from the room, everybody will. "
																	   "Continue anyway ?"),
										QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
		if(response == QMessageBox::Yes){
			connection->setShowError(false);

			server->deleteLater();
			server = NULL;
		}
	}
	if(this->isConnected() && response == QMessageBox::Yes){
		resetAll();
	}
}

void MainWindow::about(){
	QMessageBox::about(this, tr("About"),
					   "<h2><b>Drawing League</b></h2>"
					   "<b>" + tr("Version :") + "</b> " + qApp->applicationVersion() + " (<a href='https://github.com/nicolasfostier/DrawingLeague/releases'>" + tr("Latest releases") + "</a>)<br/>" +
					   "<b>" + tr("Developped by :") + "</b> <a href='https://github.com/nicolasfostier'>Nicolas Fostier</a><br/>" +
					   "<b>" + tr("Library used :") + "</b> Qt 5.9.1<br/>" +
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


void MainWindow::updateDrawingTools(){
	this->refreshPenLabel();
	if(isArtist()){
		canvasView->getCanvas()->setTools(selectedDrawingToolType(), penWidthSlider->value(), selectedColor, penLabelPixmap);
	}
	else{
		canvasView->getCanvas()->setTools(selectedDrawingToolType(), penWidthSlider->value(), selectedColor, NULL);
	}
}

void MainWindow::changeColor(){
	selectedColor = QColorDialog::getColor(selectedColor, this, tr("Pick a color"));
	this->penLabelBrush.setColor(selectedColor);
	this->updateDrawingTools();

	if(this->isConnected()){
		connection->getDBW()->sendDrawingToolColor(selectedColor);
	}
}

void MainWindow::resetCanvas(bool askConfirmation){
	int response = QMessageBox::Yes;
	if(isArtist() && askConfirmation){
		response = QMessageBox::warning(this, tr("Reset the canvas"),
										tr("Do you really want to reset the canvas ?"),
										QMessageBox::No|QMessageBox::Yes, QMessageBox::No);
	}
	if(response == QMessageBox::Yes){
		canvasView->getCanvas()->reset();
	}
}


void MainWindow::resetAll(){
	canvasView->displayCanvas();

	if(connection != NULL){
		connection->deleteLater();
		connection = NULL;

		this->toggleJoinCreateLeave();
	}

	this->resetCanvas(false);

	this->answerLineEdit->clear();
	this->answerLineEdit->setDisabled(true);

	this->answersTextEdit->clear();
	this->answersTextEdit->setDisabled(true);

	this->playersTable->setDisabled(true);

	this->chatTextEdit->clear();
	this->chatTextEdit->setDisabled(true);

	this->chatLineEdit->clear();
	this->chatLineEdit->setDisabled(true);

	roomInfo->setRoom(Room());
	roomInfo->timerStop();

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


void MainWindow::newRoom(Room room){
	this->roomInfo->setRoom(room);

	bool oneHasFound = false;

	answerLineEdit->setDisabled(false);
	answersTextEdit->setDisabled(false);
	playersTable->setDisabled(false);
	chatLineEdit->setDisabled(false);
	chatTextEdit->setDisabled(false);

	if(room.getCurrentRound() != 0){
		Player* player;
		foreach(player, players){
			if(player->getAnswerFound() == true){
				mpTicTac->play();
				oneHasFound = true;
				break;
			}
			if(player->getIsArtist()){
				artist = player;
			}
		}
	}
	else{
		answerLineEdit->setDisabled(true);
		answersTextEdit->setDisabled(true);
		canvasView->displayGameNotStarted();
	}

	this->roomInfo->timerStart(oneHasFound, room.getTimeRemainingMs());
}


void MainWindow::gameStarting(){
	canvasView->displayCanvas();

	Player* player;
	foreach(player, players){
		player->setScore(0);
	}

	updateDrawingTools();

	resetCanvas();
}

void MainWindow::roundStarting(int round, QString artist, QString word, int pointToWin){
	answerLineEdit->setDisabled(false);
	answersTextEdit->setDisabled(false);

	this->roomInfo->setRound(round);
	this->roomInfo->setArtist(artist);
	this->roomInfo->setWord(word);
	this->roomInfo->setPointToWin(pointToWin);
	this->roomInfo->timerStart(false);

	this->artist = players.find(artist).value();
	this->artist->setIsArtist(true);
	this->artist->updateColor();
	updateArtistMode();

	if(isArtist()){
		this->word = word;
		mpStartEndSkip->setMedia(QUrl("qrc:/sound/you_are_the_artist.mp3"));
	}
	else{
		mpStartEndSkip->setMedia(QUrl("qrc:/sound/new_round.mp3"));
	}

	playerFoundAnswer = 0;
	hintGiven = 0;

	mpStartEndSkip->play();
}

void MainWindow::roundEnding(QString word){
	changeDrawingToolType(DrawingToolType::PEN);
	changeDrawingToolColor(Qt::black);
	changeDrawingToolWidth(2);

	mpTicTac->stop();

	if(playerFoundAnswer == 0 && roomInfo->getPointToWin() != -1){
		if(artist != NULL){
			artist->setScore(artist->getScore() - 1);
		}

		Message msg(QString(),
					"<b><span style='color: #df2020'><i>" + roomInfo->getArtist() + "</i> " + tr("has failed to draw the word in time and lose 1 point.") + "</span></b>");
		addAnswer(msg);

		mpStartEndSkip->setMedia(QUrl("qrc:/sound/artist_has_failed_skipped.mp3"));
		mpStartEndSkip->play();
	}

	if(artist != NULL){
		artist->setIsArtist(false);
	}
	roomInfo->setArtist(" ");
	roomInfo->setPointToWin(0);
	roomInfo->timerStop();

	updateArtistMode();

	answerLineEdit->clear();
	answerLineEdit->setDisabled(true);
	answersTextEdit->setDisabled(true);

	canvasView->getCanvas()->reset();

	Player* player;
	foreach(player, players){
		player->setAnswerFound(false);
		player->updateColor();
	}

	Message msg(QString(),
				"<b><span style='color: #df2020'>" +tr("The word was") + " <i>" + word + ".</i></span></b>");
	addAnswer(msg);
}

void MainWindow::gameEnding(QString winner){
	roomInfo->setRound(0);
	roomInfo->setArtist(" ");
	roomInfo->setWord(" ");
	roomInfo->setPointToWin(0);
	roomInfo->timerStop();

	canvasView->displayEndGame(winner);

	mpStartEndSkip->setMedia(QUrl("qrc:/sound/end_game.mp3"));
	mpStartEndSkip->play();
}


void MainWindow::answerFound(QString pseudo, int pointWon){
	Player* player = players.find(pseudo).value();
	player->setScore(player->getScore() + pointWon);

	playersTable->sortByColumn(0, Qt::AscendingOrder);
	playersTable->sortByColumn(1, Qt::DescendingOrder);

	if((!isArtist() && connection->getPseudo() == pseudo) || (isArtist() && playerFoundAnswer == 0)){
		mpAnswer->setMedia(QUrl("qrc:/sound/answer_found_you.mp3"));
	}
	else{
		mpAnswer->setMedia(QUrl("qrc:/sound/answer_found_other.mp3"));
	}

	if(!player->getIsArtist()){
		Message msg;
		if(connection->getPseudo() == pseudo){
			msg = Message(QString(), "<b><span style='color: #df2020'>" + tr("You found the word !") + "</span></b>");
			answerLineEdit->clear();
			answerLineEdit->setDisabled(true);
		}
		else{
			msg = Message(QString(), "<b><span style='color: #df2020'><i>" + pseudo + "</i> " + tr("has found the word !") + "</span></b>");
		}
		addAnswer(msg);

		++playerFoundAnswer;

		if(roomInfo->getPointToWin() > 5){
			roomInfo->setPointToWin(roomInfo->getPointToWin() - 1);
		}

		player->setAnswerFound(true);
		player->updateColor();

		mpAnswer->play();
	}

	if(mpTicTac->state() == QMediaPlayer::StoppedState){
		resetAction->setDisabled(true);
		skipWordAction->setDisabled(true);
		hintAction->setDisabled(true);
		mpTicTac->play();
		roomInfo->timerStart(true);
	}
}

void MainWindow::answerClose(){
	Message msg = Message(QString(),
						  "<b><span style='color: #df2020'>" + tr("You are close !") + "</span></b>");
	addAnswer(msg);
}


void MainWindow::displayHint(QString hint){
	roomInfo->setWord(hint);

	++hintGiven;
	roomInfo->setPointToWin(roomInfo->getPointToWin() - 1);

	if(isArtist() && (roomInfo->getPointToWin() <= 5 || hintGiven > (word.size() / 2))){
		hintAction->setDisabled(true);
	}

	Message msg = Message(QString(),
						  "<b><span style='color: #df2020'>" + tr("A hint has been given. The number of point to win for this round has been decreased by 1.") + "</span></b>");
	addAnswer(msg);

	mpHint->play();
}

void MainWindow::skipWord(){
	mpStartEndSkip->setMedia(QUrl("qrc:/sound/artist_has_failed_skipped.mp3"));
	mpStartEndSkip->play();

	if(artist != NULL){
		artist->setScore(artist->getScore() - 1);
	}

	Message msg(QString(),
				"<b><span style='color: #df2020'><i>" + roomInfo->getArtist() + "</i> " + tr("has skip the word by losing 1 point.") + "</span></b>");
	addAnswer(msg);

	roomInfo->setPointToWin(-1);
}


void MainWindow::addEnteringPlayer(QString pseudo){
   addOnlinePlayer(Player(pseudo));

   Message msg = Message(QString(),
						 "<b><span style='color: #309dcf'><i>" + pseudo + "</i> " + tr("has joined the room.") + "</span></b>");
   addChat(msg);

   mpEnteringLeaving->setMedia(QUrl("qrc:/sound/player_entering.mp3"));
   mpEnteringLeaving->play();
}

void MainWindow::addOnlinePlayer(Player player){
   Player* newPlayer = new Player(player);
   newPlayer->updateColor();

   players.insert(player.getPseudo(), newPlayer);

   newPlayer->addToTableWidget(this->playersTable);
}

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

	Message msg = Message(QString(),
						  "<b><span style='color: #309dcf'><i>" + pseudo + "</i> " + tr("has left the room.") + "</span></b>");
	addChat(msg);

	mpEnteringLeaving->setMedia(QUrl("qrc:/sound/player_leaving.mp3"));
	mpEnteringLeaving->play();
}


void MainWindow::addAnswer(Message msg){
	this->answersTextEdit->append(msg.toString(false));
}

void MainWindow::addChat(Message msg){
	this->chatTextEdit->append(msg.toString(true));
}


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

void MainWindow::changeDrawingToolColor(QColor color){
	selectedColor = color;
	this->penLabelBrush.setColor(selectedColor);
	this->updateDrawingTools();
}

void MainWindow::changeDrawingToolWidth(int width){
	QObject::disconnect(penWidthSlider, SIGNAL(valueChanged(int)),
						connection->getDBW(), SLOT(sendDrawingToolWidth(int)));
	this->penWidthSlider->setValue(width);
	QObject::connect(penWidthSlider, SIGNAL(valueChanged(int)),
					 connection->getDBW(), SLOT(sendDrawingToolWidth(int)));
}


void MainWindow::sendAnswer(){
	if(!this->answerLineEdit->text().isEmpty()){
		if(this->isConnected()){
			Message msg(connection->getPseudo(), this->answerLineEdit->text());
			connection->getDBW()->sendAnswer(msg);
		}

		this->answerLineEdit->clear();

		QScrollBar* sb = answersTextEdit->verticalScrollBar();
		sb->setValue(sb->maximum());
	}
}

void MainWindow::sendChat(){
	if(!this->chatLineEdit->text().isEmpty()){
		if(this->isConnected()){
			Message msg(connection->getPseudo(), this->chatLineEdit->text());
			connection->getDBW()->sendChat(msg);
		}

		this->chatLineEdit->clear();

		QScrollBar* sb = chatTextEdit->verticalScrollBar();
		sb->setValue(sb->maximum());
	}
}


void MainWindow::sendDrawingToolType(){
	if(this->isConnected()){
		connection->getDBW()->sendDrawingToolType(selectedDrawingToolType());
	}
}
