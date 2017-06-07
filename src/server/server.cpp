#include "include/server/server.h"



// Constructor
Server::Server(int port, Room room, QString dictionaryPath)
{
    this->port = port;
    this->room = room;
    this->dictionaryPath = dictionaryPath;
    playerFoundAnswer = 0;

    drawingToolType = DrawingToolType::PEN;
    drawingToolColor = QColor(Qt::black);
    drawingToolWidth = 2;

    // Move the object to another thread and start its execution
    QThread* threadCFU = new QThread();
    QObject::connect(threadCFU, SIGNAL(finished()), threadCFU, SLOT(deleteLater()));
    this->moveToThread(threadCFU);
    QObject::connect(threadCFU, SIGNAL(started()), this, SLOT(launch()));
    threadCFU->start();
}



// Destructor
Server::~Server(){
    delete timerGameNotStarted;
    delete timerPause;
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
void Server::loadDictionary(){
    QFile dictionary(dictionaryPath);

    if(!dictionary.open(QIODevice::ReadOnly)){
        emit loadDictionaryFailed();
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

        emit loadDictionarySucceed();
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
ServerThread* Server::getNextArtist(){
    //
    if(artistsQueue.empty()){
        ServerThread* serverThread;
        foreach(serverThread, serverThreads){
            artistsQueue.append(serverThread);
        }
        std::random_shuffle(artistsQueue.begin(), artistsQueue.end());
    }

    return artistsQueue.takeFirst();
}

//
QString Server::getNextWord(){
    //
    if(wordsQueue.empty()){
        wordsQueue = QList<QString>(words);
        std::random_shuffle(wordsQueue.begin(), wordsQueue.end());
    }

    return wordsQueue.takeFirst();
}



// Qt slots

//
void Server::launch(){
    srand(time(NULL));

    //
    this->listen(QHostAddress::Any, port);

    loadDictionary();

    QObject::connect(this, SIGNAL(newConnection()), this, SLOT(addPlayer()));

    timerGameNotStarted = new QTimer();
    timerGameNotStarted->setSingleShot(true);
    timerGameNotStarted->setInterval(30000);
    QObject::connect(timerGameNotStarted, SIGNAL(timeout()), this, SLOT(sendGameNotStarted()));
    timerGameNotStarted->start();

    timerPause = new QTimer();
    timerPause->setSingleShot(true);
    timerPause->setInterval(1000);
    QObject::connect(timerPause, SIGNAL(timeout()), this, SLOT(startRound()));

    timerRound = new QTimer();
    timerRound->setSingleShot(true);
    timerRound->setInterval(room.getTimeByRound() * 1000);

    timerRoundAfterFirstAnswer = new QTimer();
    timerRoundAfterFirstAnswer->setSingleShot(true);
    timerRoundAfterFirstAnswer->setInterval(room.getTimeAfterFirstGoodAnswer() * 1000);
}


//
void Server::startRound(){
    //
    if(playerFoundAnswer > 0 || room.getRound() == 0){
        room.setRound(room.getRound() + 1);
    }

    // Select the artist
    ServerThread* nextArtist = getNextArtist();
    room.setArtist(nextArtist->getPlayer()->getPseudo());

    // Select the word
    word = getNextWord();

    // Reset the number of point to win
    room.setPointToWin(10);

    //
    playerFoundAnswer = 0;

    //
    ServerThread* serverThread;
    DataBlockWriter* svThDBW;
    foreach(serverThread, serverThreads){
        serverThread->getPlayer()->setAnswerFound(false);

        svThDBW = serverThread->getDataBlockWriter();
        if(serverThread->getPlayer()->getPseudo() == room.getArtist()){
            QMetaObject::invokeMethod(svThDBW, "sendRoundStarting", Q_ARG(quint32, quint32(room.getRound())), Q_ARG(QString, room.getArtist()), Q_ARG(QString, word), Q_ARG(quint32, quint32(10)));
        }
        else{
            QMetaObject::invokeMethod(svThDBW, "sendRoundStarting", Q_ARG(quint32, quint32(room.getRound())), Q_ARG(QString, room.getArtist()), Q_ARG(QString, room.getWord()), Q_ARG(quint32, quint32(10)));
        }
    }

    //
    QObject::connect(timerRound, SIGNAL(timeout()), this, SLOT(endRound()));
    timerRound->start();
}

//
void Server::endRound(){
    if(room.getRound() == room.getMaxRounds()){
        this->endGame();
    }
    else{
        QObject::disconnect(timerRound, SIGNAL(timeout()), this, SLOT(endRound()));
        this->timerRound->stop();
        QObject::disconnect(timerRoundAfterFirstAnswer, SIGNAL(timeout()), this, SLOT(endRound()));
        this->timerRoundAfterFirstAnswer->stop();

        timerPause->start();
    }
}

//
void Server::endGame(){

}


//
void Server::addPlayer(){
    //
    ServerThread* newServerThread = new ServerThread(nextPendingConnection());
    //
    QObject::connect(newServerThread, SIGNAL(readyToReceive(ServerThread*)), this, SLOT(setupPlayer(ServerThread*)));

    //
    QObject::connect(newServerThread, SIGNAL(playerLeaving(QString)), this, SLOT(removePlayer(QString)));

    //
    QObject::connect(this, SIGNAL(destroyed(QObject*)), newServerThread, SLOT(deleteLater()));
}
    //
    void Server::setupPlayer(ServerThread* newServerThread){
        DataBlockReader* newSvThDBR = newServerThread->getDataBlockReader();
        DataBlockWriter* newSvThDBW = newServerThread->getDataBlockWriter();

        //
        QObject::connect(newSvThDBR, SIGNAL(chatReceived(Message)), newSvThDBW, SLOT(sendChat(Message)));
        QObject::connect(newSvThDBR, SIGNAL(chatReceived(Message)), this, SLOT(checkChatCommand(Message)));
        QObject::connect(newSvThDBR, SIGNAL(answerReceived(Message)), newSvThDBW, SLOT(sendAnswer(Message)));
        QObject::connect(newSvThDBR, SIGNAL(answerReceived(Message)), this, SLOT(checkAnswer(Message)));

        //
        QObject::connect(newSvThDBR, SIGNAL(skipWordReceived()), newSvThDBW, SLOT(sendSkipWord()));
        QObject::connect(newSvThDBR, SIGNAL(skipWordReceived()), this, SLOT(skipWord()));

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
            QObject::connect(serverThread, SIGNAL(playerLeaving(QString)), newSvThDBW, SLOT(sendPlayerLeaving(QString)));
            QObject::connect(newServerThread, SIGNAL(playerLeaving(QString)), svThDBW, SLOT(sendPlayerLeaving(QString)));

            //
            QObject::connect(svThDBR, SIGNAL(chatReceived(Message)), newSvThDBW, SLOT(sendChat(Message)));
            QObject::connect(newSvThDBR, SIGNAL(chatReceived(Message)), svThDBW, SLOT(sendChat(Message)));
            //
            QObject::connect(svThDBR, SIGNAL(answerReceived(Message)), newSvThDBW, SLOT(sendAnswer(Message)));
            QObject::connect(newSvThDBR, SIGNAL(answerReceived(Message)), svThDBW, SLOT(sendAnswer(Message)));

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
            QMetaObject::invokeMethod(serverThread->getDataBlockWriter(), "sendPlayerEntering", Q_ARG(Player, *newServerThread->getPlayer()));
        }

        //
        serverThreads.insert(newServerThread->getPlayer()->getPseudo(), newServerThread);

        //
        foreach(serverThread, serverThreads){
            QMetaObject::invokeMethod(newSvThDBW, "sendPlayerEntering", Q_ARG(Player, *serverThread->getPlayer()));
        }

        //
        QMetaObject::invokeMethod(newSvThDBW, "sendRoom", Q_ARG(Room, room));
        QMetaObject::invokeMethod(newSvThDBW, "sendDrawingToolType", Q_ARG(DrawingToolType, drawingToolType));
        QMetaObject::invokeMethod(newSvThDBW, "sendDrawingToolColor", Q_ARG(QColor, drawingToolColor));
        QMetaObject::invokeMethod(newSvThDBW, "sendDrawingToolWidth", Q_ARG(int, drawingToolWidth));

        artistsQueue.append(newServerThread);
        std::random_shuffle(artistsQueue.begin(), artistsQueue.end());

        this->sendGameNotStarted();
    }

//
void Server::removePlayer(QString pseudo){
    serverThreads.remove(pseudo);
    if(pseudo == room.getArtist()){
        this->endRound();
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
void Server::skipWord(){
    if(playerFoundAnswer == 0){
        startRound();
    }
}


//
void Server::checkAnswer(Message msg){
    if(room.getRound() > 0){
        ServerThread* serverThreadAnswer = serverThreads.find(msg.getPseudo()).value();
        if(!serverThreadAnswer->getPlayer()->getAnswerFound()){
            if(msg.getMessage().toLower() == word.toLower()){

                int pointWonByArtist;
                if(playerFoundAnswer == 0){
                    pointWonByArtist = 10;

                    QObject::disconnect(timerRound, SIGNAL(timeout()), this, SLOT(endRound()));
                    this->timerRound->stop();

                    QObject::connect(this->timerRoundAfterFirstAnswer, SIGNAL(timeout()), this, SLOT(endRound()));
                    this->timerRoundAfterFirstAnswer->start();
                }
                else{
                    pointWonByArtist = 1;
                }

                serverThreadAnswer->getPlayer()->setAnswerFound(true);
                playerFoundAnswer++;

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

                // Check if everybody have found the word
                if(playerFoundAnswer == serverThreads.size() - 1){
                    this->endRound();
                }
            }
        }
    }
}

//
void Server::checkChatCommand(Message msg){
    if(room.getRound() == 0){
        if(msg.getMessage() == QString("!ready")){
            ServerThread* serverThread = serverThreads.find(msg.getPseudo()).value();
            serverThread->getPlayer()->setIsReady(true);

            //
            if(howManyMoreReadyNeeded() < 1){
                startRound();
            }
            else{
                this->sendGameNotStarted();
            }
        }
    }
}


//
void Server::sendGameNotStarted(){
    //
    if(room.getRound() == 0){
        //
        ServerThread* serverThread;
        foreach(serverThread, serverThreads){
            //
            if(!serverThread->getPlayer()->getIsReady()){
                QMetaObject::invokeMethod(serverThread->getDataBlockWriter(), "sendServerMsgTypeReady");

            }

            //
            int howManyMoreReadyNeededResult = howManyMoreReadyNeeded();
            QMetaObject::invokeMethod(serverThread->getDataBlockWriter(), "sendServerMsgReadyNeeded", Q_ARG(int, howManyMoreReadyNeededResult));
        }

        timerGameNotStarted->start();
    }
}