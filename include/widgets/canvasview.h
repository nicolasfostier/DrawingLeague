#ifndef CANVASVIEW_H
#define CANVASVIEW_H



#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QResizeEvent>
#include <QTextDocument>


#include "include/graphicsitem/readybutton.h"
#include "include/graphicsitem/canvas.h"



// QGraphicsView which display the canvas
class CanvasView : public QGraphicsView
{
	Q_OBJECT

	// Variable
	public :
		QRectF bounds;

		QGraphicsScene* canvasScene;
			Canvas* canvas;
		QGraphicsScene* gameNotStartedScene;
			QPointF readyButtonPosGNS;
		QGraphicsScene* endGameScene;
			QGraphicsTextItem* winnerPseudo;
			QGraphicsTextItem* winnerText;
			QPointF readyButtonPosEG;

			QGraphicsTextItem* readyNeeded;
			ReadyButton* readyButton;

	// Getter
	public :
		Canvas* getCanvas();
		ReadyButton* getReadyButton();

	// Constructor
	public :
		CanvasView(QRectF bounds, QWidget* parent);

	// Destructor
	public :
		~CanvasView();

	// Methods
	public :
		void displayCanvas();
		void displayGameNotStarted();
		void displayEndGame(QString winner);

		int heightForWidth(int w) const;
		void resizeEvent(QResizeEvent * event);

	// Qt slots
	public slots :
		void fit();

		void updateReadyNeeded(int nbReadyNeeded);
};



#endif // CANVASVIEW_H
