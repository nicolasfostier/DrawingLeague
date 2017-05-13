#ifndef ABSTRACTSHAPE_H
#define ABSTRACTSHAPE_H



#include <QPen>



// Abstract class which contains the shape drawn by the artist
class AbstractShape
{
    // Variables
    protected :
        // Pen to draw the shape
        QPen pen;

    // Setter
    public :
        void setPen(QPen pen);

    // Constructor
    public :
        AbstractShape(QPen pen);

    // Destructor
    public :
        virtual ~AbstractShape();

    // Methods
    public :
        // Draw the shape
        virtual void draw(QPainter* painter) = 0;
};



#endif // ABSTRACTSHAPE_H
