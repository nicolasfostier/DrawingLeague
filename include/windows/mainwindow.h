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


// Canvas where the artist can draw
#include "include/widgets/canvas.h"
//
#include "include/windows/joinroomwindow.h"
//
#include "include/windows/createroomwindow.h"



// Main window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Variables
    private :
        // Socket to discuss with the server hosting the room
        QTcpSocket* socket;

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
                    // Represent the pen
                    QLabel* penLabel;
                        // Pixmap to represent the pen
                        QPixmap* penLabelPixmap;
                        // Painter to draw the pen
                        QPainter* penLabelPainter;
                        // Brush to draw the pen
                        QBrush penLabelBrush;

                    // Regroup the drawing tool the artist can use
                    QActionGroup* drawToolsActionGroup;
                        // Pen
                        QAction* penAction;
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
                    QAction* resetAction;
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

    // Methods
    public :
        // Return the selected draw tool type
        DrawToolType selectedDrawToolType();
        // Refresh the representation of the current pen
        void refreshCurrentPenLabel();

    // Qt slots
    public slots :
        // Open a window to enter the information of the room the player want to join
        void joinRoom();
        // Open a window to set up a server
        void createRoom();

        // Update the tools set for the canvas
        void updateDrawingTools();
        // Open a dialog to change the color of the drawing tools
        void changeColor();
        // Ask if the artist really want to reset the canvas
        void resetCanvas();
};

#endif // MAINWINDOW_H
