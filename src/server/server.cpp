#include "include/server/server.h"



// Constructor
Server::Server(int port, Room room, QString dictionaryPath)
{
    if(port < 1 || port > 65535){
        qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                    << "The port number must be between 1 and 65535. Your value :"
                    << port;
        throw std::invalid_argument(tr("The port number must be between 1 and 65535.").toUtf8());
    }
    this->port = port;
    this->room = room;
    this->playerFoundAnswer = 0;
    this->artist = NULL;

    drawingToolType = DrawingToolType::PEN;
    drawingToolColor = QColor(Qt::black);
    drawingToolWidth = 2;

    this->dictionaryPath = dictionaryPath;
    loadDictionary(dictionaryPath);

    // Move the object to another thread and start its execution
    QThread* threadCFU = new QThread();
    QObject::connect(threadCFU, SIGNAL(finished()), threadCFU, SLOT(deleteLater()));
    this->moveToThread(threadCFU);
    threadCFU->start();
}



// Destructor
Server::~Server(){
    delete timerRound;
    delete timerRoundAfterFirstAnswer;

    ServerThread* serverThread;
    foreach(serverThread, serverThreads){
        serverThread->deleteLater();
    }

    // Delete its thread with it
    this->thread()->quit();
}



// Methods

//
void Server::loadDictionary(QString dictionaryPath){
    QFile dictionary(dictionaryPath);

    if(!dictionary.open(QIODevice::ReadOnly)){
        qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                    << "Impossible to load the dictionary :"
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


//
int Server::playerReady(){
    ServerThread* serverThread;
    int totalReady = 0;
    foreach(serverThread, serverThreads){
        if(serverThread->getPlayer()->getIsReady()){
            totalReady++;
        }
    }

    return totalReady;
}

//
int Server::howManyMoreReadyNeeded(){
    int result;
    if(serverThreads.size() < 2){
        result = 2 - playerReady();
    }
    else{
        result = (serverThreads.size() / 2) + 1 - playerReady();
    }

    return result;
}


//
void Server::nextArtist(){
    //
    if(artist != NULL){
        artist->getPlayer()->setIsArtist(false);
    }

    //
    artist = artistsQueue.takeFirst();
    artist->getPlayer()->setIsArtist(true);

    //
    artistsQueue.append(artist);
}

//
void Server::nextWord(){
    //
    if(wordsQueue.empty()){
        wordsQueue = QList<QString>(words);
        std::random_shuffle(wordsQueue.begin(), wordsQueue.end());
    }

    word = wordsQueue.takeFirst();
}



// Qt slots

//
void Server::launch(){
    srand(time(NULL));

    //
    this->listen(QHostAddress::Any, port);

    //
    QObject::connect(this, SIGNAL(newConnection()), this, SLOT(addPlayer()));

    //
    timerBetweenRound = new QTimer();
    timerBetweenRound->setSingleShot(true);
    timerBetweenRound->setInterval(2000);
    QObject::connect(timerBetweenRound, SIGNAL(timeout()), this, SLOT(startRound()));

    //
    timerRound = new QTimer();
    timerRound->setSingleShot(true);
    timerRound->setInterval(room.getTimeByRound() * 1000);

    //
    timerRoundAfterFirstAnswer = new QTimer();
    timerRoundAfterFirstAnswer->setSingleShot(true);
    timerRoundAfterFirstAnswer->setInterval(room.getTimeAfterFirstGoodAnswer() * 1000);

    //
    emit isReady();

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << "The server is up and ready with these settings :\n"
            << "Room name :" << room.getRoomName() << "\n"
            << "Port :" << port << "\n"
            << "Max players :" << room.getMaxPlayers() << "\n"
            << "Dictionary :" << dictionaryPath << "\n"
            << "Time by round :" << room.getTimeByRound() << "\n"
            << "Time after the first good answer :" << room.getTimeAfterFirstGoodAnswer();
}


//
void Server::startGame(){
    //
    std::random_shuffle(wordsQueue.begin(), wordsQueue.end());

    //
    ServerThread* serverThread;
    foreach(serverThread, serverThreads){
        serverThread->getPlayer()->setScore(0);
    }

    //s
    DataBlockWriter* svThDBW;
    foreach(serverThread, serverThreads){
        svThDBW = serverThread->getDataBlockWriter();
        QMetaObject::invokeMethod(svThDBW, "sendGameStarting");
    }

    //
    startRound();

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << "New game started";
}

//
void Server::startRound(){
    //
    nextWord();
    nextArtist();

    //
    room.setCurrentRound(room.getCurrentRound() + 1);
    room.setPointToWin(10);
    room.setArtist(artist->getPlayer()->getPseudo());

    //
    QObject::connect(timerRound, SIGNAL(timeout()), this, SLOT(endRound()));
    timerRound->start();

    //
    ServerThread* serverThread;
    DataBlockWriter* svThDBW;
    foreach(serverThread, serverThreads){
        svThDBW = serverThread->getDataBlockWriter();
        if(serverThread == artist){
            QMetaObject::invokeMethod(svThDBW, "sendRoundStarting", Q_ARG(quint32, quint32(room.getCurrentRound())), Q_ARG(QString, room.getArtist()), Q_ARG(QString, word), Q_ARG(quint32, quint32(10)));
        }
        else{
            QMetaObject::invokeMethod(svThDBW, "sendRoundStarting", Q_ARG(quint32, quint32(room.getCurrentRound())), Q_ARG(QString, room.getArtist()), Q_ARG(QString, room.getWord()), Q_ARG(quint32, quint32(10)));
        }
    }

    //
    playerFoundAnswer = 0;
    hintGiven = 0;

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << "Round n°" << QString::number(room.getCurrentRound()).toUtf8().data() << "starting :"
            << "Artist :" << room.getArtist()
            << "Word :" << word;
}

//
void Server::endRound(){
    //
    ServerThread* serverThread;
    DataBlockWriter* svThDBW;
    foreach(serverThread, serverThreads){
        svThDBW = serverThread->getDataBlockWriter();
        QMetaObject::invokeMethod(svThDBW, "sendRoundEnding", Q_ARG(QString, word));

        serverThread->getPlayer()->setAnswerFound(false);
    }

    QObject::disconnect(timerRound, SIGNAL(timeout()), this, SLOT(endRound()));
    this->timerRound->stop();
    QObject::disconnect(timerRoundAfterFirstAnswer, SIGNAL(timeout()), this, SLOT(endRound()));
    this->timerRoundAfterFirstAnswer->stop();

    //
    if(playerFoundAnswer == 0){
        room.setCurrentRound(room.getCurrentRound() - 1);
        if(room.getPointToWin() != -1 && artist != NULL){
            artist->getPlayer()->setScore(artist->getPlayer()->getScore() - 1);

            qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                    << room.getArtist() << "has failed to draw the word in time";
        }
    }

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << "End of the round n°" << QString::number(room.getCurrentRound()).toUtf8().data();

    if(room.getCurrentRound() == room.getNumberOfRounds()){
        this->endGame();
    }
    else{
        timerBetweenRound->start();
    }
}

//
void Server::skipWord(){
    if(artist != NULL){
        artist->getPlayer()->setScore(artist->getPlayer()->getScore() - 1);
    }

    room.setPointToWin(-1);

    endRound();

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << room.getArtist() << "has skipped the round";
}


//
void Server::endGame(){
    //
    ServerThread* serverThread;
    DataBlockWriter* svThDBW;
    Player* player;
    QString winner;
    int winnerScore = -1;
    foreach(serverThread, serverThreads){
        svThDBW = serverThread->getDataBlockWriter();
        player = serverThread->getPlayer();
        player->setIsReady(false);
        if(player->getScore() > winnerScore){
            winner = player->getPseudo();
            winnerScore = player->getScore();
        }
    }

    foreach(serverThread, serverThreads){
        svThDBW = serverThread->getDataBlockWriter();
        QMetaObject::invokeMethod(svThDBW, "sendGameEnding", Q_ARG(QString, winner));
    }

    room.setCurrentRound(0);
    room.setArtist(" ");
    room.setWord(" ");
    room.setPointToWin(0);

    sendGameNotStarted();

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << "End of the game. The winner is :" << winner;
}


//
void Server::addPlayer(){
    //
    QTcpSocket* newCon = nextPendingConnection();
    ServerThread* newServerThread = new ServerThread(newCon);

    //
    QObject::connect(newServerThread, SIGNAL(pseudoReceived(ServerThread*)), this, SLOT(setupPlayer(ServerThread*)));

    //
    QObject::connect(this, SIGNAL(destroyed(QObject*)), newServerThread, SLOT(deleteLater()));

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                << newCon->peerAddress().toString() << newCon->peerPort()
                << ": New connection attempt";
}
    //
    void Server::setupPlayer(ServerThread* newServerThread){
        //
        if(serverThreads.contains(newServerThread->getPlayer()->getPseudo())){
            qWarning() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                        << newServerThread->getTcpSocket()->peerAddress().toString() << newServerThread->getTcpSocket()->peerPort()
                        << ": Connection attempt failed : The pseudo"
                        << newServerThread->getPlayer()->getPseudo()
                        << "is already used";

            QMetaObject::invokeMethod(newServerThread, "pseudoAlreadyUsed");
        }
        else{
            //
            DataBlockReader* newSvThDBR = newServerThread->getDataBlockReader();
            DataBlockWriter* newSvThDBW = newServerThread->getDataBlockWriter();

            //
            QMetaObject::invokeMethod(newSvThDBW, "sendPseudoOk");

            //
            QObject::connect(newSvThDBR, SIGNAL(chatReceived(Message)), newSvThDBW, SLOT(sendChat(Message)));
            QObject::connect(newSvThDBR, SIGNAL(chatReceived(Message)), this, SLOT(checkChatCommand(Message)));
            QObject::connect(newSvThDBR, SIGNAL(answerReceived(Message)), newSvThDBW, SLOT(sendAnswer(Message)));
            QObject::connect(newSvThDBR, SIGNAL(answerReceived(Message)), this, SLOT(checkAnswer(Message)));

            //
            QObject::connect(newSvThDBR, SIGNAL(skipWordReceived()), newSvThDBW, SLOT(sendSkipWord()));
            QObject::connect(newSvThDBR, SIGNAL(skipWordReceived()), this, SLOT(skipWord()));

            //
            QObject::connect(newSvThDBR, SIGNAL(hintReceived(QString)), this, SLOT(hint()));

            //
            QObject::connect(newSvThDBR, SIGNAL(drawingToolTypeReceived(DrawingToolType)), this, SLOT(updateDrawingToolType(DrawingToolType)));
            QObject::connect(newSvThDBR, SIGNAL(drawingToolColorReceived(QColor)), this, SLOT(updateDrawingToolColor(QColor)));
            QObject::connect(newSvThDBR, SIGNAL(drawingToolWidthReceived(int)), this, SLOT(updateDrawingToolWidth(int)));

            //
            ServerThread* serverThread;
            DataBlockReader* svThDBR;
            DataBlockWriter* svThDBW;
            foreach(serverThread, serverThreads){
                svThDBR = serverThread->getDataBlockReader();
                svThDBW = serverThread->getDataBlockWriter();

                //
                QObject::connect(serverThread, SIGNAL(playerLeaving(QString,ServerThread*)), newSvThDBW, SLOT(sendPlayerLeaving(QString)));
                QObject::connect(newServerThread, SIGNAL(playerLeaving(QString,ServerThread*)), svThDBW, SLOT(sendPlayerLeaving(QString)));

                //
                QObject::connect(svThDBR, SIGNAL(chatReceived(Message)), newSvThDBW, SLOT(sendChat(Message)));
                QObject::connect(newSvThDBR, SIGNAL(chatReceived(Message)), svThDBW, SLOT(sendChat(Message)));

                //
                QObject::connect(svThDBR, SIGNAL(drawingToolTypeReceived(DrawingToolType)), newSvThDBW, SLOT(sendDrawingToolType(DrawingToolType)));
                QObject::connect(newSvThDBR, SIGNAL(drawingToolTypeReceived(DrawingToolType)), svThDBW, SLOT(sendDrawingToolType(DrawingToolType)));
                //
                QObject::connect(svThDBR, SIGNAL(drawingToolColorReceived(QColor)), newSvThDBW, SLOT(sendDrawingToolColor(QColor)));
                QObject::connect(newSvThDBR, SIGNAL(drawingToolColorReceived(QColor)), svThDBW, SLOT(sendDrawingToolColor(QColor)));
                //
                QObject::connect(svThDBR, SIGNAL(drawingToolWidthReceived(int)), newSvThDBW, SLOT(sendDrawingToolWidth(int)));
                QObject::connect(newSvThDBR, SIGNAL(drawingToolWidthReceived(int)), svThDBW, SLOT(sendDrawingToolWidth(int)));

                //
                QObject::connect(svThDBR, SIGNAL(canvasResetReceived()), newSvThDBW, SLOT(sendCanvasReset()));
                QObject::connect(newSvThDBR, SIGNAL(canvasResetReceived()), svThDBW, SLOT(sendCanvasReset()));

                //
                QObject::connect(svThDBR, SIGNAL(skipWordReceived()), newSvThDBW, SLOT(sendSkipWord()));
                QObject::connect(newSvThDBR, SIGNAL(skipWordReceived()), svThDBW, SLOT(sendSkipWord()));

                //
                QObject::connect(svThDBR, SIGNAL(canvasMousePressEventReceived(QPoint)), newSvThDBW, SLOT(sendCanvasMousePressEvent(QPoint)));
                QObject::connect(newSvThDBR, SIGNAL(canvasMousePressEventReceived(QPoint)), svThDBW, SLOT(sendCanvasMousePressEvent(QPoint)));
                //
                QObject::connect(svThDBR, SIGNAL(canvasMouseMoveEventReceived(QPoint)), newSvThDBW, SLOT(sendCanvasMouseMoveEvent(QPoint)));
                QObject::connect(newSvThDBR, SIGNAL(canvasMouseMoveEventReceived(QPoint)), svThDBW, SLOT(sendCanvasMouseMoveEvent(QPoint)));
                //
                QObject::connect(svThDBR, SIGNAL(canvasMouseReleaseEventReceived(QPoint)), newSvThDBW, SLOT(sendCanvasMouseReleaseEvent(QPoint)));
                QObject::connect(newSvThDBR, SIGNAL(canvasMouseReleaseEventReceived(QPoint)), svThDBW, SLOT(sendCanvasMouseReleaseEvent(QPoint)));

                //
                QMetaObject::invokeMethod(svThDBW, "sendPlayerEntering", Q_ARG(Player, *newServerThread->getPlayer()));
                QMetaObject::invokeMethod(newSvThDBW, "sendPlayerOnline", Q_ARG(Player, *serverThread->getPlayer()));
            }

            //
            QMetaObject::invokeMethod(newSvThDBW, "sendPlayerEntering", Q_ARG(Player, *newServerThread->getPlayer()));

            //
            QMetaObject::invokeMethod(newSvThDBW, "sendDrawingToolType", Q_ARG(DrawingToolType, drawingToolType));
            QMetaObject::invokeMethod(newSvThDBW, "sendDrawingToolColor", Q_ARG(QColor, drawingToolColor));
            QMetaObject::invokeMethod(newSvThDBW, "sendDrawingToolWidth", Q_ARG(int, drawingToolWidth));

            //
            if(timerRound->remainingTime() != -1 || timerRound->remainingTime() == 0){
                room.setTimeRemaining(timerRound->remainingTime() / 1000);
            }
            else if (timerRoundAfterFirstAnswer->remainingTime() != -1){
                room.setTimeRemaining(timerRoundAfterFirstAnswer->remainingTime() / 1000);
            }
            else{
                room.setTimeRemaining(0);
            }
            QMetaObject::invokeMethod(newSvThDBW, "sendRoom", Q_ARG(Room, room));

            //
            serverThreads.insert(newServerThread->getPlayer()->getPseudo(), newServerThread);
            //
            QObject::connect(newServerThread, SIGNAL(playerLeaving(QString,ServerThread*)), this, SLOT(removePlayer(QString,ServerThread*)));

            artistsQueue.append(newServerThread);

            this->sendGameNotStarted();

            qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                        << newServerThread->getTcpSocket()->peerAddress().toString() << newServerThread->getTcpSocket()->peerPort()
                        << ": Player entering :"
                        << newServerThread->getPlayer()->getPseudo();
        }
    }

//
void Server::removePlayer(QString pseudo, ServerThread* serverThread){
    serverThreads.remove(pseudo);
    if(pseudo == room.getArtist()){
        artist = NULL;
        this->endRound();
    }

    artistsQueue.removeOne(serverThread);

    //
    if(serverThreads.isEmpty()){
        endGame();
    }

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << "Player leaving :"
            << pseudo;
}


//
void Server::checkAnswer(Message msg){
    if(room.getCurrentRound() > 0){
        ServerThread* serverThreadAnswer = serverThreads.find(msg.getPseudo()).value();
        qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                << serverThreadAnswer->getTcpSocket()->peerAddress().toString() << serverThreadAnswer->getTcpSocket()->peerPort()
                << ": Answer :"
                << msg.toString(true);
        if(!serverThreadAnswer->getPlayer()->getAnswerFound() && !serverThreadAnswer->getPlayer()->getIsArtist()){
            if(msg.getMessage().toLower() == word.toLower()){

                int pointWonByArtist;
                if(playerFoundAnswer == 0){
                    pointWonByArtist = room.getPointToWin();

                    QObject::disconnect(timerRound, SIGNAL(timeout()), this, SLOT(endRound()));
                    this->timerRound->stop();

                    QObject::connect(this->timerRoundAfterFirstAnswer, SIGNAL(timeout()), this, SLOT(endRound()));
                    this->timerRoundAfterFirstAnswer->start();
                }
                else{
                    pointWonByArtist = 1;
                }

                serverThreadAnswer->getPlayer()->setAnswerFound(true);
                serverThreadAnswer->getPlayer()->setScore(serverThreadAnswer->getPlayer()->getScore() + room.getPointToWin());
                if(artist != NULL){
                    artist->getPlayer()->setScore(artist->getPlayer()->getScore() + pointWonByArtist);
                }
                playerFoundAnswer++;

                qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                        << serverThreadAnswer->getPlayer()->getPseudo() << "has found the word and won" << room.getPointToWin();

                // Send point win
                ServerThread* serverThread;
                DataBlockWriter* svThDBW;
                foreach(serverThread, serverThreads){
                    svThDBW = serverThread->getDataBlockWriter();
                    QMetaObject::invokeMethod(svThDBW, "sendAnswerFound", Q_ARG(QString, room.getArtist()), Q_ARG(quint32, pointWonByArtist));
                    QMetaObject::invokeMethod(svThDBW, "sendAnswerFound", Q_ARG(QString, serverThreadAnswer->getPlayer()->getPseudo()), Q_ARG(quint32, room.getPointToWin()));
                }

                if(room.getPointToWin() > 5){
                    room.setPointToWin(room.getPointToWin() - 1);
                }

                //
                QMetaObject::invokeMethod(artist->getDataBlockWriter(), "sendAnswer", Q_ARG(Message, msg));

                // Check if everybody have found the word
                if(playerFoundAnswer == serverThreads.size() - 1){
                    this->endRound();
                }
            }
            else{
                //
                ServerThread* serverThread;
                DataBlockWriter* svThDBW;
                foreach(serverThread, serverThreads){
                    if(serverThread->getPlayer()->getPseudo() != msg.getPseudo()){
                        svThDBW = serverThread->getDataBlockWriter();
                        QMetaObject::invokeMethod(svThDBW, "sendAnswer", Q_ARG(Message, msg));
                    }
                }
            }
        }
    }
}

//
void Server::checkChatCommand(Message msg){
    ServerThread* serverThread = serverThreads.find(msg.getPseudo()).value();

    qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
            << serverThread->getTcpSocket()->peerAddress().toString() << serverThread->getTcpSocket()->peerPort()
            << ": Chat :"
            << msg.toString(true);

    if(room.getCurrentRound() == 0){
        if(msg.getMessage() == QString("!ready")){
            serverThread->getPlayer()->setIsReady(true);

            qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                    << serverThread->getTcpSocket()->peerAddress().toString() << serverThread->getTcpSocket()->peerPort()
                    << serverThread->getPlayer()->getPseudo() << "is now ready.";

            //
            if(howManyMoreReadyNeeded() < 1){
                startGame();
            }
            else{
                this->sendGameNotStarted();
            }
        }
    }
}


//
void Server::hint(){
    //
    if(playerFoundAnswer == 0 && room.getPointToWin() > 5 && hintGiven <= (word.size() / 2)){
        //
        QString hint = word.mid(0, hintGiven);
        for(int i = 0; i < word.size() - hintGiven; i++){
            if(i == 0 && hintGiven == 0)
            {
                hint.append("_");
            }
            else{
                hint.append(" _");
            }
        }

        //
        DataBlockWriter* svThDBW;
        ServerThread* serverThread;
        foreach(serverThread, serverThreads){
            if(serverThread != artist){
                svThDBW = serverThread->getDataBlockWriter();
                QMetaObject::invokeMethod(svThDBW, "sendHint", Q_ARG(QString, hint));
            }
        }

        //
        svThDBW = artist->getDataBlockWriter();
        QMetaObject::invokeMethod(svThDBW, "sendHint", Q_ARG(QString, word + " " + hint));

        //
        hintGiven++;
        room.setPointToWin(room.getPointToWin() - 1);

        qInfo() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
                << "A hint has been given";
    }
}


//
void Server::updateDrawingToolType(DrawingToolType drawingToolType){
    this->drawingToolType = drawingToolType;
}

//
void Server::updateDrawingToolColor(QColor color){
    this->drawingToolColor = color;
}

//
void Server::updateDrawingToolWidth(int width){
    this->drawingToolWidth = width;
}


//
void Server::sendGameNotStarted(){
    //
    if(room.getCurrentRound() == 0){
        //
        ServerThread* serverThread;
        foreach(serverThread, serverThreads){
            //
            int howManyMoreReadyNeededResult = howManyMoreReadyNeeded();
            QMetaObject::invokeMethod(serverThread->getDataBlockWriter(), "sendServerMsgReadyNeeded", Q_ARG(int, howManyMoreReadyNeededResult));
        }
    }
}
