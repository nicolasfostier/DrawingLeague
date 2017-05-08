#include "include/windows/mainwindow.h"



// Constructor
MainWindow::MainWindow() : QMainWindow()
{
    // Initialization of the settings variable
    settings = new QSettings(this);


    // Room menu
    menuRoom = menuBar()->addMenu(tr("Room"));
        actionJoin = menuRoom->addAction(QIcon(), tr("Join a room"));
        actionCreate = menuRoom->addAction(QIcon(), tr("Create a room"));
        actionQuit = menuRoom->addAction(QIcon(), tr("Quit"));
        QObject::connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Tools menu
    menuTools = menuBar()->addMenu(tr("Tools"));
        actionSettings = menuTools->addAction(QIcon(), tr("Settings"));

    // Help menu
    menuHelp = menuBar()->addMenu(tr("?"));
        actionAbout = menuHelp->addAction(QIcon(), tr("About"));

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
            gameInfoWidget = new QWidget(mainWidget);
            gameInfoWidget->setContentsMargins(0,0,0,0);
            gameInfoWidget->setStyleSheet(".QWidget{border: 1px solid grey; background: white}");
            mainLayout->addWidget(gameInfoWidget, 0, 0, 1, 3);

                // Its layout
                gameInfoLayout = new QGridLayout(gameInfoWidget);
                gameInfoLayout->setColumnStretch(0, 1);
                gameInfoLayout->setColumnStretch(1, 1);
                gameInfoLayout->setColumnStretch(2, 1);
                gameInfoLayout->setColumnStretch(3, 1);
                gameInfoLayout->setColumnStretch(4, 1);
                gameInfoLayout->setColumnStretch(5, 0);

                    // Contain the name of the room
                    roomNameLabel = new QLabel(tr("Room name : "), mainWidget);
                    gameInfoLayout->addWidget(roomNameLabel, 0, 0, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

                    // Display the progress of the game
                    roundLabel = new QLabel(tr("Round : X/X"), mainWidget);
                    gameInfoLayout->addWidget(roundLabel, 0, 1, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

                    // Display the artist name
                    artistLabel = new QLabel(tr("Artist : "), mainWidget);
                    gameInfoLayout->addWidget(artistLabel, 0, 2, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

                    // Display the word to guess or some hint about this word
                    wordToGuessHintLabel = new QLabel(tr("Word : "), mainWidget);
                    gameInfoLayout->addWidget(wordToGuessHintLabel, 0, 3, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

                    // Point to win during the current round
                    pointToWinLabel = new QLabel(tr("Point to win : X"), mainWidget);
                    gameInfoLayout->addWidget(pointToWinLabel, 0, 4, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

                    // Time remaining before the round ends
                    timeTextLabel = new QLabel(tr("Time : "), mainWidget);
                    gameInfoLayout->addWidget(timeTextLabel, 0, 5, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

                    // Time remaining before the round ends represented with a circle
                    timeCircleLabel = new QLabel(mainWidget);
                    gameInfoLayout->addWidget(timeCircleLabel, 0, 6, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);

            // Toolbar which contains all the tool to draw on the canvas for the artist
            drawToolsBar = new QToolBar(mainWidget);
            drawToolsBar->setOrientation(Qt::Vertical);
            mainLayout->addWidget(drawToolsBar, 1, 0, 1, 1);

                drawToolsActionGroup = new QActionGroup(drawToolsBar);

                    // Pen
                    penAction = drawToolsActionGroup->addAction(QIcon(), tr("Free line"));
                    penAction->setCheckable(true);
                    penAction->setChecked(true);

                    // Eraser
                    eraserAction = drawToolsActionGroup->addAction(QIcon(), tr("Eraser"));
                    eraserAction->setCheckable(true);

                drawToolsBar->addActions(drawToolsActionGroup->actions());
                drawToolsBar->addSeparator();

                // Reset
                resetAction = drawToolsBar->addAction(QIcon(), tr("Reset the canvas"));
                resetAction->setCheckable(false);

            // The canvas, where the artist can draw
            canvasLabel = new QLabel(mainWidget);
            canvasLabel->setPixmap(QPixmap(600,600));
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
                            newAnswersLineEdit = new QLineEdit(answersWidget);
                            answersLayout->addWidget(newAnswersLineEdit);

                    // Players
                    playersWidget = new QWidget(answersPlayersSplitter);

                        playersLayout = new QVBoxLayout(playersWidget);
                        playersLayout->setContentsMargins(0,0,0,0);

                            // Title
                            playersTitle = new QLabel("<center><b>" + tr("Players :") + "</b></center>", playersWidget);
                            playersLayout->addWidget(playersTitle);

                            // List of all players connected to the room
                            playersTextEdit = new QTableWidget(answersPlayersSplitter);
                            playersLayout->addWidget(playersTextEdit);

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
                        newMessageLineEdit = new QLineEdit(answersPlayersChatSplitter);
                        chatLayout->addWidget(newMessageLineEdit);

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

}
