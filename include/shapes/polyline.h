#ifndef POLYLINE_H
#define POLYLINE_H



#include <QPolygon>
#include <QPainter>


// Abstract class which contains the shape drawn by the artist
#include "include/shapes/shape.h"



// A polyline which can be drawn by the artist
class Polyline : public AbstractShape
{
    // Variables
    private :
        // The points of the polyline is stored in this polygon
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
        // Draw the shape
        void draw(QPainter *painter);
        // Add a point at the end of the polyline
        void addPoint(QPoint newPoint);
};



#endif // POLYLINE_H
