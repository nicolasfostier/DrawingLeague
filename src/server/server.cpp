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
			++totalReady;
		}
	}

	return totalReady;
}

int Server::nbReadyNeeded(){
	/* Temporary force 100% of the players to be ready.
	 * But when after i implement a feature to skip afk player in the futur,
	 * it will be more than 50 % of ready players needed.
	 *
	 * And i will maybe add the possibility to custom this % in the server settings
	 * to overwrite the result of this function.
	*/
	if(players.size() < 2){
		return 2 - playerReady();
	}
	else{
		return players.size() - playerReady();
	}
//	if(players.size() < 2){
//		result = 2 - playerReady();
//	}
//	else{
//		result = (players.size() / 2) + 1 - playerReady();
//	}
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


void Server::incomingConnection(qintptr socketDescriptor){
	ServerThread* newPlayer = new ServerThread(socketDescriptor);

	QObject::connect(newPlayer, SIGNAL(wantToEnterTheGame()),
					 this, SLOT(verifyNewPlayer()));

	QObject::connect(this, SIGNAL(destroyed(QObject*)),
					 newPlayer, SLOT(deleteLater()));

	newPlayer->thread()->start();
}
	void Server::verifyNewPlayer(){
		ServerThread* newPlayer = static_cast<ServerThread*>(sender());

		QString newPlayerPseudo = newPlayer->getPlayer()->getPseudo();

		if(players.contains(newPlayerPseudo)){
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
			players.insert(newPlayerPseudo, newPlayer);

			QObject::connect(newPlayer, SIGNAL(readyToReceive()),
							 this, SLOT(setupNewPlayer()));
			QMetaObject::invokeMethod(newPlayer, "hasEnteredTheGame");

			if(discPlayersScore.contains(newPlayerPseudo)){
				newPlayer->getPlayer()->setScore(discPlayersScore.value(newPlayerPseudo));
				discPlayersScore.remove(newPlayerPseudo);
			}
		}
	}
	void Server::setupNewPlayer(){
		ServerThread* newPlayer = static_cast<ServerThread*>(sender());

		SocketReader* newPlayerSReader = newPlayer->getSocketReader();
		SocketWriter* newPlayerSWriter = newPlayer->getSocketWriter();

		QObject::connect(newPlayerSReader, SIGNAL(chatReceived(Message)),
						 this, SLOT(processChatCommand(Message)));
		QObject::connect(newPlayerSReader, SIGNAL(answerReceived(Message)),
						 this, SLOT(processAnswer(Message)));

		QObject::connect(newPlayerSReader, SIGNAL(hintReceived(QString)),
						 this, SLOT(hint()));
		QObject::connect(newPlayerSReader, SIGNAL(skipWordReceived()),
						 this, SLOT(skipWord()));

		QObject::connect(newPlayerSReader, SIGNAL(readyReceived()),
						 this, SLOT(setPlayerReady()));

		QObject::connect(newPlayerSReader, SIGNAL(drawingToolTypeReceived(DrawingToolType)),
						 this, SLOT(changeDrawingToolType(DrawingToolType)));
		QObject::connect(newPlayerSReader, SIGNAL(drawingToolColorReceived(QColor)),
						 this, SLOT(changeDrawingToolColor(QColor)));
		QObject::connect(newPlayerSReader, SIGNAL(drawingToolWidthReceived(int)),
						 this, SLOT(changeDrawingToolWidth(int)));

		QObject::connect(newPlayerSReader, SIGNAL(canvasResetReceived()),
						 this, SLOT(canvasReset()));

		QObject::connect(newPlayerSReader, SIGNAL(canvasMousePressEventReceived(QPoint)),
						 this, SLOT(canvasMousePressEvent(QPoint)));
		QObject::connect(newPlayerSReader, SIGNAL(canvasMouseMoveEventReceived(QPoint)),
						 this, SLOT(canvasMouseMoveEvent(QPoint)));
		QObject::connect(newPlayerSReader, SIGNAL(canvasMouseReleaseEventReceived(QPoint)),
						 this, SLOT(canvasMouseReleaseEvent(QPoint)));

		QObject::connect(newPlayer, SIGNAL(playerLeaving(Player, ServerThread*)),
						 this, SLOT(removePlayer(Player, ServerThread*)));


		ServerThread* player;
		SocketWriter* playerSWriter;
		foreach(player, players){
			playerSWriter = player->getSocketWriter();

			QObject::connect(player, SIGNAL(playerLeaving(Player, ServerThread*)),
							 newPlayerSWriter, SLOT(sendPlayerLeaving(Player)));
			QObject::connect(newPlayer, SIGNAL(playerLeaving(Player, ServerThread*)),
							 playerSWriter, SLOT(sendPlayerLeaving(Player)));

			if(player != newPlayer){
				QMetaObject::invokeMethod(playerSWriter, "sendPlayerEntering",
										  Q_ARG(Player, *newPlayer->getPlayer()));
				QMetaObject::invokeMethod(newPlayerSWriter, "sendPlayerOnline",
										  Q_ARG(Player, *player->getPlayer()));
			}
		}

		QMetaObject::invokeMethod(newPlayerSWriter, "sendPlayerEntering",
								  Q_ARG(Player, *newPlayer->getPlayer()));

		QMetaObject::invokeMethod(newPlayerSWriter, "sendDrawingToolType",
								  Q_ARG(DrawingToolType, drawingToolType));
		QMetaObject::invokeMethod(newPlayerSWriter, "sendDrawingToolColor",
								  Q_ARG(QColor, drawingToolColor));
		QMetaObject::invokeMethod(newPlayerSWriter, "sendDrawingToolWidth",
								  Q_ARG(int, drawingToolWidth));

		if(timerRound->remainingTime() != -1){
			room.setTimeRemainingMs(timerRound->remainingTime());
		}
		else{
			room.setTimeRemainingMs(0);
		}
		QMetaObject::invokeMethod(newPlayerSWriter, "sendRoom",
								  Q_ARG(Room, room));

		artistsQueue.append(newPlayer);

		this->sendReadyNeeded();

		qInfo() << newPlayer->getSocket()->peerAddress().toString() << newPlayer->getSocket()->peerPort()
				<< ": Player entering :"
				<< newPlayer->getPlayer()->getPseudo();
	}

void Server::removePlayer(Player player, ServerThread* playerTh){
	players.remove(player.getPseudo());
	artistsQueue.removeOne(playerTh);

	qInfo()	<< "Player leaving :"
			<< player.getPseudo();

	if(room.getCurrentRound() == 0){
		if(nbReadyNeeded() < 1){
			startGame();
		}
		else{
			this->sendReadyNeeded();
		}
	}
	else{


		if(player.getPseudo() == room.getArtist()){
			artist = NULL;
			this->endRound();

			discPlayersScore.insert(player.getPseudo(), player.getScore() - 1);
		}
		else{
			discPlayersScore.insert(player.getPseudo(), player.getScore());

			if(players.size() < 2){
				endGame();
			}
			else if(player.getAnswerFound()){
				playerFoundAnswer--;
			}
		}
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

		ServerThread* player;
		if(!playerSender->getPlayer()->getAnswerFound() && !playerSender->getPlayer()->getIsArtist()){
			if(removeAccents(msg.getMessage().toLower()) == removeAccents(word.toLower())){

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
				++playerFoundAnswer;

				qInfo()	<< playerSender->getPlayer()->getPseudo() << "has found the word and won" << room.getPointToWin();

				foreach(player, players){
					QMetaObject::invokeMethod(player->getSocketWriter(), "sendAnswerFound",
											  Q_ARG(QString, room.getArtist()),
											  Q_ARG(quint32, pointWonByArtist));
					QMetaObject::invokeMethod(player->getSocketWriter(), "sendAnswerFound",
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
			else if(isClose(word, msg.getMessage())){
				QMetaObject::invokeMethod(playerSender->getSocketWriter(), "sendAnswerClose");

				foreach(player, players){
					if(player->getPlayer()->getAnswerFound() || player == playerSender || player == artist){
						QMetaObject::invokeMethod(player->getSocketWriter(), "sendAnswer",
												  Q_ARG(Message, msg));
					}
				}
			}
			else{
				foreach(player, players){
						QMetaObject::invokeMethod(player->getSocketWriter(), "sendAnswer",
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
		QMetaObject::invokeMethod(player->getSocketWriter(), "sendChat",
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
	   !oneHasFound && room.getPointToWin() > 5 && hintGiven * words.size() <= (word.size() / 2)){

		QString hint = QString();

		int i;
		int iTot = 0;
		foreach(QString word, words){
			hint.append(word.mid(0, hintGiven));
			int charRemaining = word.size() - hintGiven;
			for(i = 0; i < charRemaining; ++i){
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
				++iTot;
			}
		}

		ServerThread* player;
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendHint",
										  Q_ARG(QString, hint));
			}
		}

		QMetaObject::invokeMethod(artist->getSocketWriter(), "sendHint",
								  Q_ARG(QString, word + " " + hint));

		++hintGiven;
		room.setPointToWin(room.getPointToWin() - 1);

		qInfo() << room.getArtist() << "has given a hint";
	}
}

void Server::skipWord(){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist && !oneHasFound){
		ServerThread* player;
		foreach(player, players){
			QMetaObject::invokeMethod(player->getSocketWriter(), "sendSkipWord");
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
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendDrawingToolType",
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
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendDrawingToolColor",
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
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendDrawingToolWidth",
										  Q_ARG(int, width));
			}
		}
	}
}

void Server::canvasReset(){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist && !oneHasFound){
		ServerThread* player;
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendCanvasReset");
			}
		}
		qInfo() << room.getArtist() << "has reset the canvas" ;
	}
}


void Server::canvasMousePressEvent(QPoint pos){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		ServerThread* player;
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendCanvasMousePressEvent",
										  Q_ARG(QPoint, pos));
			}
		}
	}
}

void Server::canvasMouseMoveEvent(QPoint pos){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		ServerThread* player;
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendCanvasMouseMoveEvent",
										  Q_ARG(QPoint, pos));
			}
		}
	}
}

void Server::canvasMouseReleaseEvent(QPoint pos){
	ServerThread* playerSender = static_cast<ServerThread*>(sender()->parent()->parent());
	if(playerSender == artist){
		ServerThread* player;
		foreach(player, players){
			if(player != artist){
				QMetaObject::invokeMethod(player->getSocketWriter(), "sendCanvasMouseReleaseEvent",
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

	ServerThread* player;
	foreach(player, players){
		player->getPlayer()->setScore(0);
		QMetaObject::invokeMethod(player->getSocketWriter(), "sendGameStarting");
	}
	discPlayersScore.clear();

	startRound();

	qInfo() << "New game started";
}

void Server::startRound(){
	nextWord();
	nextArtist();

	if(oneHasFound || room.getCurrentRound() == 0){
		room.setCurrentRound(room.getCurrentRound() + 1);
	}
	room.setPointToWin(10);
	room.setArtist(artist->getPlayer()->getPseudo());

	QObject::connect(this->timerRound, SIGNAL(timeout()),
					 this, SLOT(endRound()));
	timerRound->setInterval(this->room.getTimeByRound() * 1000);
	timerRound->start();

	ServerThread* player;
	foreach(player, players){
		if(player == artist){
			QMetaObject::invokeMethod(player->getSocketWriter(), "sendRoundStarting",
									  Q_ARG(quint32, quint32(room.getCurrentRound())),
									  Q_ARG(QString, room.getArtist()),
									  Q_ARG(QString, word),
									  Q_ARG(quint32, quint32(10)));
		}
		else{
			QMetaObject::invokeMethod(player->getSocketWriter(), "sendRoundStarting",
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

	ServerThread* player;
	foreach(player, players){
		QMetaObject::invokeMethod(player->getSocketWriter(), "sendRoundEnding",
								  Q_ARG(QString, word));

		player->getPlayer()->setAnswerFound(false);
	}

	QObject::disconnect(this->timerRound, SIGNAL(timeout()),
						this, SLOT(endRound()));
	this->timerRound->stop();

	if(!oneHasFound){
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

	if(room.getCurrentRound() == room.getNumberOfRounds() && oneHasFound){
		this->endGame();
	}
	else{
		timerBetweenRound->start();
	}
}

void Server::endGame(){
	ServerThread* playerThread;
	Player* player;
	QString winner = "?";
	int winnerScore = -999999;
	foreach(playerThread, players){
		player = playerThread->getPlayer();
		player->setIsReady(false);
		if(player->getScore() > winnerScore){
			qInfo() << player->getPseudo() << player->getScore();
			winner = player->getPseudo();
			winnerScore = player->getScore();
		}
	}

	foreach(playerThread, players){
		QMetaObject::invokeMethod(playerThread->getSocketWriter(), "sendGameEnding",
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
			QMetaObject::invokeMethod(player->getSocketWriter(), "sendReadyNeeded",
									  Q_ARG(int, nbReadyNeededResult));
		}
	}
}
