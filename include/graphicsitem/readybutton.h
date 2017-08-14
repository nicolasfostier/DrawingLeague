#ifndef READYBUTTON_H
#define READYBUTTON_H



#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QCursor>



// Ready button to start a game
class ReadyButton : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

	// Variables
	private :
		bool isReady;

		bool isMouseHover;
		bool isMousePressed;

		QRectF rect;

		QPen pen;
		QBrush brush;

	// Setter
	public :
		void setIsReady(bool isReady);

	// Constructor
	public :
		ReadyButton();

	// Destructor
	public :
		~ReadyButton();

	// Methods
	public :
		void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	// Qt signals
	signals :
		void ready();
};



#endif // READYBUTTON_H
