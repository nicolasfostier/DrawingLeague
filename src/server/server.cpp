#include "include/server/server.h"



// Constructor
Server::Server(int port, Room room, QString dictionaryPath)
{
	if(port < 1 || port > 65535){
		qCritical() << "The port number must be between 1 and 65535. Your value :"
					<< port;
		throw std::invalid_argument(tr("The port number must be between 1 and 65535.").toUtf8());
	}
	this->port = port;
	this->room = room;
	this->playerFoundAnswer = 0;
	this->artist = NULL;

	drawingToolType = DrawingToolType::PEN;
	drawingToolColor = QColor(Qt::black);
	drawingToolWidth = 3;

	this->dictionaryPath = dictionaryPath;
	loadDictionary(dictionaryPath);

	// Move the object to another thread and start its execution
	QThread* threadCFU = new QThread();
	QObject::connect(threadCFU, SIGNAL(finished()),
					 threadCFU, SLOT(deleteLater()));
	QObject::connect(threadCFU, SIGNAL(started()),
					 this, SLOT(launch()));
	this->moveToThread(threadCFU);
}



// Destructor
Server::~Server(){
	qCritical()
				<< "The server is closing";

	this->thread()->quit();
}



// Methods

void Server::loadDictionary(QString dictionaryPath){
	QFile dictionary(dictionaryPath);

	if(!dictionary.open(QIODevice::ReadOnly)){
		qCritical()	<< "Impossible to load the dictionary :"
					<< dictionaryPath;
		throw std::invalid_argument(tr("Impossible to load the dictionary.").toUtf8());
	}
	else{
		QTextStream dictionaryStream(&dictionary);
		dictionaryStream.setCodec("UTF-8");
		while(!dictionaryStream.atEnd()){
			QString word = dictionaryStream.readLine();
			if(!word.isEmpty()){
				words.append(word);
			}
		}

		wordsQueue = QList<QString>(words);
		std::random_shuffle(wordsQueue.begin(), wordsQueue.end());
	}
}


int Server::playerReady(){
	ServerThread* serverThread;
	int totalReady = 0;
	foreach(serverThread, players){
		if(serverThread->getPlayer()->getIsReady()){
			totalReady++;
		}
	}

	return totalReady;
}

int Server::nbReadyNeeded(){
	int result;
	if(players.size() < 2){
		result = 2 - playerReady();
	}
	else{
		result = (players.size() / 2) + 1 - playerReady();
	}

	return result;
}


void Server::nextArtist(){
	artist = artistsQueue.takeFirst();
	artist->getPlayer()->setIsArtist(true);

	artistsQueue.append(artist);
}

void Server::nextWord(){
	//
	if(wordsQueue.empty()){
		wordsQueue = QList<QString>(words);
		std::random_shuffle(wordsQueue.begin(), wordsQueue.end());
	}

	word = wordsQueue.takeFirst();
}



// Qt slots

void Server::launch(){
	srand(time(NULL));

	this->listen(QHostAddress::Any, port);

	QObject::connect(this, SIGNAL(newConnection()),
					 this, SLOT(handleNewConnection()));

	timerBetweenRound = new QTimer();
	timerBetweenRound->setSingleShot(true);
	timerBetweenRound->setInterval(2000);

	timerRound = new QTimer(this);
	timerRound->setSingleShot(true);

	emit isReady();

	qInfo()	<< "The server is up and ready with these settings :\n"
			<< "Room name :" << room.getRoomName() << "\n"
			<< "Port :" << port << "\n"
			<< "Max players :" << room.getMaxPlayers() << "\n"
			<< "Dictionary :" << dictionaryPath << "\n"
			<< "Time by round :" << room.getTimeByRound() << "\n"
			<< "Time after the first good answer :" << room.getTimeAfterFirstGoodAnswer();
}


void Server::handleNewConnection(){
	QTcpSocket* newCon = nextPendingConnection();
	ServerThread* newPlayer = new ServerThread(newCon);

	QObject::connect(newPlayer, SIGNAL(wantToEnterTheGame()),
					 this, SLOT(verifyNewPlayer()));

	QObject::connect(this, SIGNAL(destroyed(QObject*)),
					 newPlayer, SLOT(deleteLater()));

	newPlayer->thread()->start();

	qInfo()	<< newCon->peerAddress().toString() << newCon->peerPort()
			<< ": New connection attempt";
}
	void Server::verifyNewPlayer(){
//		qInfo() << "Avant le cast";
		ServerThread* newPlayer = static_cast<ServerThread*>(sender());
//		qInfo() << "Après le cast";
		qInfo() << newPlayer->getPlayer()->getPseudo();

		if(players.contains(newPlayer->getPlayer()->getPseudo())){
			qWarning() << newPlayer->getSocket()->peerAddress().toString() << newPlayer->getSocket()->peerPort()
					   << ": Connection attempt failed : The pseudo"
					   << newPlayer->getPlayer()->getPseudo()
					   << "is already used";

			QMetaObject::invokeMethod(newPlayer, "cantEnterTheGame",
									  Q_ARG(ErrorCode, ErrorCode::PSEUDO_ALREADY_TAKEN));
		}
		else if(newPlayer->getGameVersion() != qApp->applicationVersion()){
			qWarning() << newPlayer->getSocket()->peerAddress().toString() << newPlayer->getSocket()->peerPort()
					   << ": Connection attempt failed : The player don't use the same version of the game than the server";

			QMetaObject::invokeMethod(newPlayer, "cantEnterTheGame",
									  Q_ARG(ErrorCode, ErrorCode::GAME_VERSION_DIFFERENT));
		}
		else if(players.size() == room.getMaxPlayers()){
			qWarning() << newPlayer->getSocket()->peerAddress().toString() << newPlayer->getSocket()->peerPort()
					   << ": Connection attempt failed : The server is full";

			QMetaObject::invokeMethod(newPlayer, "cantEnterTheGame",
									  Q_ARG(ErrorCode, ErrorCode::SERVER_FULL));
		}
		else{
			players.insert(newPlayer->getPlayer()->getPseudo(), newPlayer);

			QObject::connect(newPlayer, SIGNAL(readyToReceive()),
							 this, SLOT(setupNewPlayer()));
			QMetaObject::invokeMethod(newPlayer, "hasEnteredTheGame");
		}
	}
	void Server::setupNewPlayer(){
		ServerThread* newPlayer = static_cast<ServerThread*>(sender());

		DataBlockReader* newPlayerDBR = newPlayer->getDataBlockReader();
		DataBlockWriter* newPlayerDBW = newPlayer->getDataBlockWriter();

		QObject::connect(newPlayerDBR, SIGNAL(chatReceived(Message)),
						 this, SLOT(processChatCommand(Message)));
		QObject::connect(newPlayerDBR, SIGNAL(answerReceived(Message)),
						 this, SLOT(processAnswer(Message)));

		QObject::connect(newPlayerDBR, SIGNAL(hintReceived(QString)),
						 this, SLOT(hint()));
		QObject::connect(newPlayerDBR, SIGNAL(skipWordReceived()),
						 this, SLOT(skipWord()));

		QObject::connect(newPlayerDBR, SIGNAL(readyReceived()),
						 this, SLOT(setPlayerReady()));

		QObject::connect(newPlayerDBR, SIGNAL(drawingToolTypeReceived(DrawingToolType)),
						 this, SLOT(changeDrawingToolType(DrawingToolType)));
		QObject::connect(newPlayerDBR, SIGNAL(drawingToolColorReceived(QColor)),
						 this, SLOT(changeDrawingToolColor(QColor)));
		QObject::connect(newPlayerDBR, SIGNAL(drawingToolWidthReceived(int)),
						 this, SLOT(changeDrawingToolWidth(int)));

		QObject::connect(newPlayerDBR, SIGNAL(canvasResetReceived()),
						 this, SLOT(canvasReset()));

		QObject::connect(newPlayerDBR, SIGNAL(canvasMousePressEventReceived(QPoint)),
						 this, SLOT(canvasMousePressEvent(QPoint)));
		QObject::connect(newPlayerDBR, SIGNAL(canvasMouseMoveEventReceived(QPoint)),
						 this, SLOT(canvasMouseMoveEvent(QPoint)));
		QObject::connect(newPlayerDBR, SIGNAL(canvasMouseReleaseEventReceived(QPoint)),
						 this, SLOT(canvasMouseReleaseEvent(QPoint)));

		QObject::connect(newPlayer, SIGNAL(playerLeaving(QString,ServerThread*,bool)),
						 this, SLOT(removePlayer(QString,ServerThread*,bool)));


		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			playerDBW = player->getDataBlockWriter();

			QObject::connect(player, SIGNAL(playerLeaving(QString,ServerThread*,bool)),
							 newPlayerDBW, SLOT(sendPlayerLeaving(QString)));
			QObject::connect(newPlayer, SIGNAL(playerLeaving(QString,ServerThread*,bool)),
							 playerDBW, SLOT(sendPlayerLeaving(QString)));

			if(player != newPlayer){
				QMetaObject::invokeMethod(playerDBW, "sendPlayerEntering",
										  Q_ARG(QString, newPlayer->getPlayer()->getPseudo()));
				QMetaObject::invokeMethod(newPlayerDBW, "sendPlayerOnline",
										  Q_ARG(Player, *player->getPlayer()));
			}
		}

		QMetaObject::invokeMethod(newPlayerDBW, "sendPlayerEntering",
								  Q_ARG(QString, newPlayer->getPlayer()->getPseudo()));

		QMetaObject::invokeMethod(newPlayerDBW, "sendDrawingToolType",
								  Q_ARG(DrawingToolType, drawingToolType));
		QMetaObject::invokeMethod(newPlayerDBW, "sendDrawingToolColor",
								  Q_ARG(QColor, drawingToolColor));
		QMetaObject::invokeMethod(newPlayerDBW, "sendDrawingToolWidth",
								  Q_ARG(int, drawingToolWidth));

		if(timerRound->remainingTime() != -1){
			room.setTimeRemainingMs(timerRound->remainingTime());
		}
		else{
			room.setTimeRemainingMs(0);
		}
		QMetaObject::invokeMethod(newPlayerDBW, "sendRoom",
								  Q_ARG(Room, room));

		artistsQueue.append(newPlayer);

		this->sendReadyNeeded();

		qInfo() << newPlayer->getSocket()->peerAddress().toString() << newPlayer->getSocket()->peerPort()
				<< ": Player entering :"
				<< newPlayer->getPlayer()->getPseudo();
	}

void Server::removePlayer(QString pseudo, ServerThread* player, bool hasFound){
	players.remove(pseudo);
	if(pseudo == room.getArtist()){
		artist = NULL;
		this->endRound();
	}
	if(hasFound){
		playerFoundAnswer--;
	}

	artistsQueue.removeOne(player);

	qInfo()	<< "Player leaving :"
			<< pseudo;

	if(players.size() < 2){
		endGame();
	}
}


void Server::processAnswer(Message msg){
	msg.escapeHTML();

	qInfo() << msg.toString(false);

	if(room.getCurrentRound() > 0 && timerRound->remainingTime() > 0){
		ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());

		qInfo() << playerSender->getSocket()->peerAddress().toString() << playerSender->getSocket()->peerPort()
				<< ": Answer :"
				<< msg.toString(false);

		if(!playerSender->getPlayer()->getAnswerFound() && !playerSender->getPlayer()->getIsArtist()){
			if(removeAccents(msg.getMessage().toLower()) == removeAccents(word.toLower())){

				QMetaObject::invokeMethod(playerSender->getDataBlockWriter(), "sendAnswer",
										  Q_ARG(Message, msg));

				QMetaObject::invokeMethod(artist->getDataBlockWriter(), "sendAnswer",
										  Q_ARG(Message, msg));

				int pointWonByArtist;
				if(!oneHasFound){
					oneHasFound = true;

					pointWonByArtist = room.getPointToWin();

					QObject::disconnect(this->timerRound, SIGNAL(timeout()),
									 this, SLOT(endRound()));

					this->timerRound->stop();

					this->timerRound->setInterval(this->room.getTimeAfterFirstGoodAnswer() * 1000);

					this->timerRound->start();
					QObject::connect(this->timerRound, SIGNAL(timeout()),
									 this, SLOT(endRound()));
				}
				else{
					pointWonByArtist = 1;
				}

				playerSender->getPlayer()->setAnswerFound(true);
				playerSender->getPlayer()->setScore(playerSender->getPlayer()->getScore() + room.getPointToWin());
				if(artist != NULL){
					artist->getPlayer()->setScore(artist->getPlayer()->getScore() + pointWonByArtist);
				}
				playerFoundAnswer++;

				qInfo()	<< playerSender->getPlayer()->getPseudo() << "has found the word and won" << room.getPointToWin();

				ServerThread* player;
				DataBlockWriter* playerDBW;
				foreach(player, players){
					playerDBW = player->getDataBlockWriter();
					QMetaObject::invokeMethod(playerDBW, "sendAnswerFound",
											  Q_ARG(QString, room.getArtist()),
											  Q_ARG(quint32, pointWonByArtist));
					QMetaObject::invokeMethod(playerDBW, "sendAnswerFound",
											  Q_ARG(QString, playerSender->getPlayer()->getPseudo()),
											  Q_ARG(quint32, room.getPointToWin()));
				}

				if(room.getPointToWin() > 5){
					room.setPointToWin(room.getPointToWin() - 1);
				}

				if(playerFoundAnswer == players.size() - 1){
					this->endRound();
				}
			}
			else if(removeAccents(msg.getMessage().toLower()).mid(0,4) == removeAccents(word.toLower().mid(0,4))){
				QMetaObject::invokeMethod(playerSender->getDataBlockWriter(), "sendAnswerClose");
				QMetaObject::invokeMethod(playerSender->getDataBlockWriter(), "sendAnswer",
										  Q_ARG(Message, msg));
				QMetaObject::invokeMethod(artist->getDataBlockWriter(), "sendAnswer",
										  Q_ARG(Message, msg));
			}
			else{
				ServerThread* player;
				DataBlockWriter* playerDBW;
				foreach(player, players){
						playerDBW = player->getDataBlockWriter();
						QMetaObject::invokeMethod(playerDBW, "sendAnswer",
												  Q_ARG(Message, msg));
				}
			}
		}
	}
}

void Server::processChatCommand(Message msg){
	msg.escapeHTML();
	ServerThread* player;
	foreach(player, players){
		QMetaObject::invokeMethod(player->getDataBlockWriter(), "sendChat",
								  Q_ARG(Message, msg));
	}

	player = players.find(msg.getPseudo()).value();

	qInfo() << player->getSocket()->peerAddress().toString() << player->getSocket()->peerPort()
			<< ": Chat :"
			<< msg.toString(false);
}


void Server::hint(){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	QStringList words = word.split(QRegExp("(\\s+|-)"), QString::SkipEmptyParts);
	if(playerSender == artist &&
	   playerFoundAnswer == 0 && room.getPointToWin() > 5 && hintGiven * words.size() <= (word.size() / 2)){

		QString hint = QString();

		int i;
		int iTot = 0;
		foreach(QString word, words){
			hint.append(word.mid(0, hintGiven));
			for(i = 0; i < word.size() - hintGiven; i++){
				if(i == 0 && hintGiven == 0){
					hint.append("_");
				}
				else{
					hint.append(" _");
				}
			}

			iTot += word.mid(0, hintGiven).size() + i;
			if(iTot != this->word.size()){
				hint.append(" " + this->word.mid(iTot, 1) + " ");
				iTot++;
			}
		}

		DataBlockWriter* playerDBW;
		ServerThread* player;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendHint",
										  Q_ARG(QString, hint));
			}
		}

		playerDBW = artist->getDataBlockWriter();
		QMetaObject::invokeMethod(playerDBW, "sendHint",
								  Q_ARG(QString, word + " " + hint));

		hintGiven++;
		room.setPointToWin(room.getPointToWin() - 1);

		qInfo() << room.getArtist() << "has given a hint";
	}
}

void Server::skipWord(){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist && playerFoundAnswer == 0){
		DataBlockWriter* playerDBW;
		ServerThread* player;
		foreach(player, players){
			playerDBW = player->getDataBlockWriter();
			QMetaObject::invokeMethod(playerDBW, "sendSkipWord");
		}

		artist->getPlayer()->setScore(artist->getPlayer()->getScore() - 1);

		room.setPointToWin(-1);
		endRound();

		qInfo() << room.getArtist() << "has skipped the round";
	}
}


void Server::setPlayerReady(){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(room.getCurrentRound() == 0){
		playerSender->getPlayer()->setIsReady(true);

		qInfo()	<< playerSender->getSocket()->peerAddress().toString() << playerSender->getSocket()->peerPort()
				<< playerSender->getPlayer()->getPseudo() << "is now ready.";

		if(nbReadyNeeded() < 1){
			startGame();
		}
		else{
			this->sendReadyNeeded();
		}
	}
}


void Server::changeDrawingToolType(DrawingToolType drawingToolType){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		this->drawingToolType = drawingToolType;

		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendDrawingToolType",
										  Q_ARG(DrawingToolType, drawingToolType));
			}
		}
	}
}

void Server::changeDrawingToolColor(QColor color){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		this->drawingToolColor = color;

		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendDrawingToolColor",
										  Q_ARG(QColor, color));
			}
		}
	}
}

void Server::changeDrawingToolWidth(int width){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		this->drawingToolWidth = width;

		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendDrawingToolWidth",
										  Q_ARG(int, width));
			}
		}
	}
}

void Server::canvasReset(){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist && playerFoundAnswer == 0){
		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendCanvasReset");
			}
		}
		qInfo() << room.getArtist() << "has reset the canvas" ;
	}
}


void Server::canvasMousePressEvent(QPoint pos){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendCanvasMousePressEvent",
										  Q_ARG(QPoint, pos));
			}
		}
	}
}

void Server::canvasMouseMoveEvent(QPoint pos){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendCanvasMouseMoveEvent",
										  Q_ARG(QPoint, pos));
			}
		}
	}
}

void Server::canvasMouseReleaseEvent(QPoint pos){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		ServerThread* player;
		DataBlockWriter* playerDBW;
		foreach(player, players){
			if(player != artist){
				playerDBW = player->getDataBlockWriter();
				QMetaObject::invokeMethod(playerDBW, "sendCanvasMouseReleaseEvent",
										  Q_ARG(QPoint, pos));
			}
		}
	}
}


void Server::startGame(){
	std::random_shuffle(wordsQueue.begin(), wordsQueue.end());
	std::random_shuffle(artistsQueue.begin(), artistsQueue.end());

	QObject::connect(this->timerBetweenRound, SIGNAL(timeout()),
						this, SLOT(startRound()));

	ServerThread* serverThread;
	foreach(serverThread, players){
		serverThread->getPlayer()->setScore(0);
	}

	DataBlockWriter* svThDBW;
	foreach(serverThread, players){
		svThDBW = serverThread->getDataBlockWriter();
		QMetaObject::invokeMethod(svThDBW, "sendGameStarting");
	}

	//
	startRound();

	qInfo() << "New game started";
}

void Server::startRound(){
	nextWord();
	nextArtist();

	if(playerFoundAnswer != 0 || room.getCurrentRound() == 0){
		room.setCurrentRound(room.getCurrentRound() + 1);
	}
	room.setPointToWin(10);
	room.setArtist(artist->getPlayer()->getPseudo());

	QObject::connect(this->timerRound, SIGNAL(timeout()),
					 this, SLOT(endRound()));
	timerRound->setInterval(this->room.getTimeByRound() * 1000);
	timerRound->start();

	ServerThread* serverThread;
	DataBlockWriter* svThDBW;
	foreach(serverThread, players){
		svThDBW = serverThread->getDataBlockWriter();
		if(serverThread == artist){
			QMetaObject::invokeMethod(svThDBW, "sendRoundStarting",
									  Q_ARG(quint32, quint32(room.getCurrentRound())),
									  Q_ARG(QString, room.getArtist()),
									  Q_ARG(QString, word),
									  Q_ARG(quint32, quint32(10)));
		}
		else{
			QMetaObject::invokeMethod(svThDBW, "sendRoundStarting",
									  Q_ARG(quint32, quint32(room.getCurrentRound())),
									  Q_ARG(QString, room.getArtist()),
									  Q_ARG(QString, room.getWord()),
									  Q_ARG(quint32, quint32(10)));
		}
	}

	playerFoundAnswer = 0;
	oneHasFound = false;
	hintGiven = 0;

	qInfo() << "Round n°" << QString::number(room.getCurrentRound()).toUtf8().data() << "starting :"
			<< "Artist :" << room.getArtist()
			<< "Word :" << word;
}

void Server::endRound(){
	drawingToolType = DrawingToolType::PEN;
	drawingToolColor = QColor(Qt::black);
	drawingToolWidth = 3;

	ServerThread* serverThread;
	DataBlockWriter* svThDBW;
	foreach(serverThread, players){
		svThDBW = serverThread->getDataBlockWriter();
		QMetaObject::invokeMethod(svThDBW, "sendRoundEnding",
								  Q_ARG(QString, word));

		serverThread->getPlayer()->setAnswerFound(false);
	}

	QObject::disconnect(this->timerRound, SIGNAL(timeout()),
						this, SLOT(endRound()));
	this->timerRound->stop();

	if(playerFoundAnswer == 0){
		room.setCurrentRound(room.getCurrentRound() - 1);
		if(room.getPointToWin() != -1 && artist != NULL){
			artist->getPlayer()->setScore(artist->getPlayer()->getScore() - 1);

			qInfo() << room.getArtist() << "has failed to draw the word in time";
		}
	}
	else{
		qInfo() << "End of the round n°" << QString::number(room.getCurrentRound()).toUtf8().data();
	}

	if(artist != NULL){
		artist->getPlayer()->setIsArtist(false);
		artist = NULL;
	}

	if(room.getCurrentRound() == room.getNumberOfRounds()){
		this->endGame();
	}
	else{
		timerBetweenRound->start();
	}
}

void Server::endGame(){
	ServerThread* serverThread;
	DataBlockWriter* svThDBW;
	Player* player;
	QString winner;
	int winnerScore = -999999;
	foreach(serverThread, players){
		svThDBW = serverThread->getDataBlockWriter();
		player = serverThread->getPlayer();
		player->setIsReady(false);
		if(player->getScore() > winnerScore){
			winner = player->getPseudo();
			winnerScore = player->getScore();
		}
	}

	foreach(serverThread, players){
		svThDBW = serverThread->getDataBlockWriter();
		QMetaObject::invokeMethod(svThDBW, "sendGameEnding",
								  Q_ARG(QString, winner));
	}

	QObject::disconnect(this->timerBetweenRound, SIGNAL(timeout()),
						this, SLOT(startRound()));
	this->timerBetweenRound->stop();
	QObject::disconnect(this->timerRound, SIGNAL(timeout()),
						this, SLOT(endRound()));
	this->timerRound->stop();

	room.setCurrentRound(0);
	room.setArtist(" ");
	room.setWord(" ");
	room.setPointToWin(0);

	sendReadyNeeded();

	qInfo() << "End of the game. The winner is :" << winner;
}


void Server::sendReadyNeeded(){
	if(room.getCurrentRound() == 0){
		ServerThread* player;
		foreach(player, players){
			int nbReadyNeededResult = nbReadyNeeded();
			QMetaObject::invokeMethod(player->getDataBlockWriter(), "sendReadyNeeded",
									  Q_ARG(int, nbReadyNeededResult));
		}
	}
}
