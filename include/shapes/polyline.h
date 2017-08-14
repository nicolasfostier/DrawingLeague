#ifndef POLYLINE_H
#define POLYLINE_H



#include <QPolygon>
#include <QPainter>


#include "include/shapes/shape.h"



// A polyline which can be drawn by the artist
class Polyline : public AbstractShape
{
	// Variables
	private :
		QPolygon polyline;

	// Getter
	public :
		QPoint getLastPoint();
		bool isFinished();

	// Constructor
	public :
		Polyline(QPen pen, QPoint firstPoint);

	// Destructor
	public :
		~Polyline();

	// Methods
	public :
		void draw(QPainter *painter);
		void addPoint(QPoint newPoint);
};



#endif // POLYLINE_H
