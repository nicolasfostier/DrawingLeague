#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QTableWidget>
#include <QToolBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QSlider>
#include <QColorDialog>
#include <QMessageBox>
#include <QSpinBox>
#include <QTcpSocket>
#include <QHeaderView>
#include <QHash>
#include <QMediaPlayer>



// Canvas where the artist can draw
#include "include/widgets/canvas.h"
//
#include "include/windows/joinroomwindow.h"
//
#include "include/windows/createroomwindow.h"
//
#include "include/datablock/datablockreader.h"
//
#include "include/datablock/datablockwriter.h"
//
#include "include/gameinfo/room.h"
//
#include "include/gameinfo/message.h"
//
#include "include/widgets/roominfo.h"
//
#include "include/gameinfo/player.h"



// Main window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Variables
    private :
        // Settings
        QSettings* settings;

        //
        QMediaPlayer* mpEnteringLeaving;
        //
        QMediaPlayer* mpStartEndSkip;
        //
        QMediaPlayer* mpAnswer;
        //
        QMediaPlayer* mpTicTac;

        //
        Server* server;

        //
        QTcpSocket* socket;
        //
        DataBlockReader* dataBlockReader;
        //
        DataBlockWriter* dataBlockWriter;

        //
        QString pseudo;
        //
        Room room;
        //
        QHash<QString, Player*> players;

        //
        QTimer* timer;
        //
        int secondCounter;

        // Menu
        QMenu* menuRoom;
            QAction* actionJoin;
            QAction* actionCreate;
            QAction* actionLeave;
            QAction* actionQuit;
        QMenu* menuTools;
            QAction* actionSettings;
        QMenu* menuHelp;
            QAction* actionAbout;

        // The main widget which contains everything else
        QWidget* mainWidget;
            // Its layout
            QGridLayout* mainLayout;
                // Display all the info about the current game
                RoomInfo* roomInfo;
                // Toolbar which contains all the tool to draw on the canvas for the artist
                QToolBar* drawingToolsBar;
                    // Represent the pen
                    QLabel* penLabel;
                        // Pixmap to represent the pen
                        QPixmap* penLabelPixmap;
                        // Painter to draw the pen
                        QPainter* penLabelPainter;
                        // Brush to draw the pen
                        QBrush penLabelBrush;
                    // Regroup the drawing tool the artist can use
                    QActionGroup* drawingToolsActionGroup;
                        // Pen
                        QAction* penAction;
                        // Bucket
                        QAction* bucketAction;
                        // Eraser
                        QAction* eraserAction;
                    // Width of the pen
                    QSpinBox* penWidthSpinBox;
                    // Width of the pen
                    QSlider* penWidthSlider;
                    // Pick the color of the pen
                    QAction* actionColor;
                        QColor selectedColor;
                    // Reset
                    QAction* actionReset;
                    // Send a hint
                    QAction* actionHint;
                    // Skip the word
                    QAction* actionSkipWord;
                // The canvas, where the artist can draw
                Canvas* canvasLabel;
                // Splitter which connects the chat with the players and the answers
                QSplitter* answersPlayersChatSplitter;
                    // Splitter witch connects the players with the answers
                    QSplitter* answersPlayersSplitter;
                        // Answers
                        QWidget* answersWidget;
                            QVBoxLayout* answersLayout;
                                // Title
                                QLabel* answersTitle;
                                // All the answers
                                QTextEdit* answersTextEdit;
                                // Where players can enter their answers
                                QLineEdit* answerLineEdit;
                        // Players
                        QWidget* playersWidget;
                            QVBoxLayout* playersLayout;
                                // Title
                                QLabel* playersTitle;
                                // List of all players connected to the room
                                QTableWidget* playersTable;

                    // Chat
                    QWidget* chatWidget;
                        QVBoxLayout* chatLayout;
                            // Title
                            QLabel* chatTitle;
                            // All the messages sent by players
                            QTextEdit* chatTextEdit;
                            // Where players can enter their messages to chat with the others players
                            QLineEdit* chatLineEdit;

    // Constructor
    public :
        MainWindow();

    // Destructor
    public :
        ~MainWindow();

    // Methods
    public :
        //
        bool isConnected();
        //
        bool isHosting();

        //
        bool isArtist();

        //
        void toggleJoinCreateLeave();
        //
        void updateArtistMode();

        // Return the selected draw tool type
        DrawingToolType selectedDrawingToolType();
        // Refresh the representation of the current pen
        void refreshPenLabel();

        //
        void closeEvent(QCloseEvent* event);

    // Qt slots
    public slots :
        // Open a window to enter the information of the room the player want to join
        void joinRoom();
            //
            void roomJoined(QTcpSocket* socket, DataBlockReader* dataBlockReader, DataBlockWriter* dataBlockWriter, QString pseudo);
        // Open a window to set up a server
        void createRoom();
            //
            void roomCreated(Server* server, QTcpSocket* socket, DataBlockReader* dataBlockReader, DataBlockWriter* dataBlockWriter, QString pseudo);
        //
        void leaveRoom();
        //
        void about();

        // Update the tools set for the canvas
        void updateDrawingTools();
        // Open a dialog to change the color of the drawing tools
        void changeColor();
        // Ask if the artist really want to reset the canvas
        void resetCanvas(bool askConfirmation = true);

        //
        void resetAll();

        //
        void newRoom(Room room);

        //
        void oneSecond();

        //
        void gameStarting();
        //
        void roundStarting(int round, QString artist, QString word, int pointToWin);
        //
        void roundEnding(QString word);
        //
        void skipWord();
        //
        void gameEnding(QString winner);

        //
        void answerFound(QString pseudo, int pointWon);

        //
        void displayHint(QString hint);

        //
        void addEnteringPlayer(Player player);
        //
        void addOnlinePlayer(Player player);
        //
        void removePlayer(QString pseudo);

        //
        void addAnswer(Message msg);
        //
        void addChat(Message msg);

        //
        void changeDrawingToolType(DrawingToolType drawingToolType);
        //
        void changeDrawingToolColor(QColor color);
        //
        void changeDrawingToolWidth(int width);

        //
        void showServerMsgReadyNeeded(int playerNotReadyNeeded);

        //
        void sendAnswer();
        //
        void sendChat();

        //
        void sendDrawingToolType();

        //
        void serverClosed();
};

#endif // MAINWINDOW_H
