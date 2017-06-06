#ifndef SERVER_H
#define SERVER_H



#include <algorithm>
#include <random>
#include <time.h>


#include <QTcpServer>
#include <QThread>
#include <QColor>
#include <QFile>


//
#include "include/server/serverthread.h"
//
#include "include/gameinfo/room.h"
//
#include "include/drawingtooltype.h"



// Host a room
class Server : public QTcpServer
{
    Q_OBJECT

    // Variables
    private :
        //
        int port;        
        //
        QHash<QString, ServerThread*> serverThreads;

        //
        Room room;

        //
        QList<ServerThread*> artistsQueue;

        //
        QString dictionaryPath;
        //
        QList<QString> words;
        //
        QList<QString> wordsQueue;
        //
        QString word;

        //
        int playerFoundAnswer;

        //
        QTimer* timerGameNotStarted;
        //
        QTimer* timerPause;
        //
        QTimer* timerRound;
        //
        QTimer* timerRoundAfterFirstAnswer;

        //
        DrawingToolType drawingToolType;
        //
        QColor drawingToolColor;
        //
        int drawingToolWidth;

    // Constructor
    public:
        Server(int port, Room room, QString dictionaryPath);

    // Destructor
    public :
        ~Server();

    // Methods
    public :
        //
        void loadDictionary();

        //
        int playerReady();
        //
        int howManyMoreReadyNeeded();

        //
        ServerThread* getNextArtist();
        //
        QString getNextWord();

    // Qt slots
    public slots :
        //
        void launch();

        //
        void startRound();
        //
        void endRound();
        //
        void endGame();

        //
        void addPlayer();
            //
            void setupPlayer(ServerThread* newServerThread);
        //
        void removePlayer(QString pseudo);

        //
        void updateDrawingToolType(DrawingToolType drawingToolType);
        //
        void updateDrawingToolColor(QColor color);
        //
        void updateDrawingToolWidth(int width);

        //
        void checkAnswer(Message msg);
        //
        void checkChatCommand(Message msg);

        //
        void sendGameNotStarted();

    // Signals
    signals :
        //
        void loadDictionarySucceed();
        //
        void loadDictionaryFailed();
};

#endif // SERVER_H
