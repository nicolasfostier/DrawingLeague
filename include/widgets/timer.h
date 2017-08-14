#ifndef TIMECIRCLE_H
#define TIMECIRCLE_H


#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QBrush>



// Display a timer as a circle / pie
class Timer : public QWidget{
	Q_OBJECT

	// Variables
	private :
		QHBoxLayout* layout;
			QLabel* label;
				QFont labelFont;
			QLabel* timerText;
				QFont timerTextFont;
				QPalette timerTextPalette;
			QLabel* timerCircle;

		int timeByRound;
		int timeAfterFirstGoodAnswer;
		int oneHasFound;

		QTimer* qTimer;
		QTimer* FPSLimiter;

		QPixmap* pixmap;
		QPainter* painter;

		QPen* pen;
		QBrush* brush;

	// Setter
	public :
		void setTimeByRound(int timeByRound);
		void setTimeAfterFirstGoodAnswer(int timeAfterFirstGoodAnswer);

	// Constructeur
	public :
		Timer(int timeByRound, int timeAfterFirstGoodAnswer);

	// Destructeur
	public :
		~Timer();

	// Methods
	public :
		void start(int timeRemainingMs, bool oneHasFound);
		void stop();

	// Qt slots
	public slots :
		void update();
};



#endif // TIMECIRCLE_H
