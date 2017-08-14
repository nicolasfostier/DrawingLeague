#ifndef CANVAS_H
#define CANVAS_H



#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QLinkedList>
#include <QCursor>
#include <QTimer>
#include <QTextOption>
#include <QImage>
#include <QStack>
#include <QIcon>
#include <QPaintEngine>


#include "include/shapes/polyline.h"
#include "include/gameinfo/drawingtooltype.h"
#include "include/datablock/datablockreader.h"
#include "include/datablock/datablockwriter.h"



// Canvas where the artist can draw
class Canvas : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

	// Variables
	private :
		bool isArtist;

		QImage image;
		QPainter* painter;
		QPen drawingPen;
		QBrush drawingBrush;
		QPen erasePen;

		DrawingToolType currentDrawingTool;

		QPointF lastMousePositionDrawn;
		QPointF lastMousePositionNotDrawn;
		QRectF lastRectDrawn;

		QTimer* FPSLimiter;
		QTimer* TickrateLimiter;

		QCursor bucketCursor;

	// Setter
	public :
		void setTools(DrawingToolType drawingToolType, int penWidth, QColor penBrushColor, QPixmap* cursorPixmap);
		void setIsArtist(bool isArtist);

	// Constructor
	public :
		Canvas(DrawingToolType currentDrawingTool, QColor penBrushColor, int penWidth, int width, int height);

	// Destructor
	public :
		~Canvas();

	// Methods
	public :
		void floodFill(QPoint point, QColor previousColor, QColor newColor);
		void reset();

		void mousePressEvent(QGraphicsSceneMouseEvent * event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	// Qt slots
	public slots :
		void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

		void mousePressEventFromServer(QPoint pos);
		void mouseMoveEventFromServer(QPoint pos);
		void mouseReleaseEventFromServer(QPoint pos);

		void refresh();
		void mapTickrateTimeoutSignal();

	// Qt signals
	signals :
		void mousePressEventToSend(QPoint pos);
		void mouseMoveEventToSend(QPoint pos);
		void mouseReleaseEventToSend(QPoint pos);
};



#endif // CANVAS_H
