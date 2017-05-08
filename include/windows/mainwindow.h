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



// Main window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Variables
    private :
        // Settings
        QSettings* settings;

        // Menu
        QMenu* menuRoom;
            QAction* actionJoin;
            QAction* actionCreate;
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
                QWidget* gameInfoWidget;
                    // Its layout
                    QGridLayout* gameInfoLayout;
                        // Contain the name of the room
                        QLabel* roomNameLabel;
                        // Display the progress of the game
                        QLabel* roundLabel;
                         // Display the artist name
                        QLabel* artistLabel;
                        // Display the word to guess or some hint about this word
                        QLabel* wordToGuessHintLabel;
                        // Point to win during the current round
                        QLabel* pointToWinLabel;
                        // Time remaining before the round ends
                        QLabel* timeTextLabel;
                        // Time remaining before the round ends represented with a circle
                        QLabel* timeCircleLabel;
                // Toolbar which contains all the tool to draw on the canvas for the artist
                QToolBar* drawToolsBar;
                    QActionGroup* drawToolsActionGroup;
                        // Pen
                        QAction* penAction;
                        // Eraser
                        QAction* eraserAction;
                        // Reset
                        QAction* resetAction;
                // The canvas, where the artist can draw
                QLabel* canvasLabel;
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
                                QLineEdit* newAnswersLineEdit;
                        // Players
                        QWidget* playersWidget;
                            QVBoxLayout* playersLayout;
                                // Title
                                QLabel* playersTitle;
                                // List of all players connected to the room
                                QTableWidget* playersTextEdit;

                    // Chat
                    QWidget* chatWidget;
                        QVBoxLayout* chatLayout;
                            // Title
                            QLabel* chatTitle;
                            // All the messages sent by players
                            QTextEdit* chatTextEdit;
                            // Where players can enter their messages to chat with the others players
                            QLineEdit* newMessageLineEdit;

    // Constructor
    public :
        MainWindow();
};

#endif // MAINWINDOW_H
