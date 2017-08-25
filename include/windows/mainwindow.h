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
#include <QJsonDocument>
#include <QJsonObject>
#include <QScrollBar>



#include "include/windows/joinroomwindow.h"
#include "include/windows/createroomwindow.h"
#include "include/gameinfo/room.h"
#include "include/gameinfo/message.h"
#include "include/gameinfo/player.h"
#include "include/widgets/roominfo.h"
#include "include/widgets/canvasview.h"
#include "include/socket/socketreader.h"
#include "include/socket/socketwriter.h"



// Main window of the program
class MainWindow : public QMainWindow
{
	Q_OBJECT

	// Variables
	private :
		QSettings* settings;

		QMediaPlayer* mpEnteringLeaving;
		QMediaPlayer* mpStartEndSkip;
		QMediaPlayer* mpAnswer;
		QMediaPlayer* mpHint;
		QMediaPlayer* mpTicTac;

		Server* server;

		Connection* connection;

		QHash<QString, Player*> players;
		Player* artist;
		QString word;
		int playerFoundAnswer;
		int hintGiven;

		QMenu* menuRoom;
			QAction* joinAction;
			QAction* createAction;
			QAction* leaveAction;
			QAction* quitAction;
		QMenu* menuTools;
			QAction* settingsAction;
		QMenu* menuHelp;
			QAction* aboutAction;

		QWidget* mainWidget;
			QGridLayout* mainLayout;
				RoomInfo* roomInfo;
				QToolBar* drawingToolsBar;
					QLabel* penLabel;
						QPixmap* penLabelPixmap;
						QPainter* penLabelPainter;
						QBrush penLabelBrush;
					QActionGroup* drawingToolsActionGroup;
						QAction* penAction;
						QAction* bucketAction;
						QAction* eraserAction;
					QSpinBox* penWidthSpinBox;
					QSlider* penWidthSlider;
					QAction* colorAction;
						QColor selectedColor;
					QAction* resetAction;
					QAction* hintAction;
					QAction* skipWordAction;
				QSplitter* mainBlockSplitter;
					CanvasView* canvasView;
					QSplitter* answersPlayersChatSplitter;
						QSplitter* answersPlayersSplitter;
							QWidget* answersWidget;
								QVBoxLayout* answersLayout;
									QLabel* answersTitle;
									QTextEdit* answersTextEdit;
									QLineEdit* answerLineEdit;
							QWidget* playersWidget;
								QVBoxLayout* playersLayout;
									QLabel* playersTitle;
									QTableWidget* playersTable;
						QWidget* chatWidget;
							QVBoxLayout* chatLayout;
								QLabel* chatTitle;
								QTextEdit* chatTextEdit;
								QLineEdit* chatLineEdit;

	// Constructor
	public :
		MainWindow();

	// Destructor
	public :
		~MainWindow();

	// Methods
	public :
		bool isConnected();
		bool isHosting();

		bool isArtist();

		void toggleJoinCreateLeave();
		void updateArtistMode();

		DrawingToolType selectedDrawingToolType();
		void refreshPenLabel();

		void closeEvent(QCloseEvent* event);

	// Qt slots
	public slots :
		void showNewUpdateAvailable(QJsonDocument jsonReply);

		void joinRoom();
			void roomJoined(Connection* connection);
		void createRoom();
			void roomCreated(Server* server, Connection* connection);
		void leaveRoom();
		void about();

		void updateDrawingTools();
		void changeColor();
		void resetCanvas(bool askConfirmation = true);

		void resetAll();

		void newRoom(Room room);

		void gameStarting();
		void roundStarting(int round, QString artist, QString word, int pointToWin);
		void roundEnding(QString word);
		void gameEnding(QString winner);

		void answerFound(QString pseudo, int pointWon);
		void answerClose();

		void displayHint(QString hint);
		void skipWord();

		void addEnteringPlayer(QString pseudo);
		void addOnlinePlayer(Player player);
		void removePlayer(QString pseudo);

		void addAnswer(Message msg);
		void addChat(Message msg);

		void changeDrawingToolType(DrawingToolType drawingToolType);
		void changeDrawingToolColor(QColor color);
		void changeDrawingToolWidth(int width);

		void sendAnswer();
		void sendChat();

		void sendDrawingToolType();
};



#endif // MAINWINDOW_H
