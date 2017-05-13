#ifndef CANVAS_H
#define CANVAS_H



#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QLinkedList>
#include <QCursor>
#include <QTimer>


// A polyline which can be drawn by the artist
#include "include/shapes/polyline.h"
// All the type of tool to draw on the canvas
#include "include/drawtooltype.h"



// Canvas where the artist can draw
class Canvas : public QLabel
{
    Q_OBJECT

    // Variables
    private :
        // Width of the canvas
        int width;
        // Height of the canvas
        int height;

        // Pixmap of the canvas
        QPixmap* pixmap;
        // Painter of the canvas
        QPainter* painter;
        // Pen to draw on the canvas
        QPen drawPen;
        // Brush to draw on the canvas (for filled stuff)
        QBrush drawBrush;
        // Pen to erase (just a white pen)
        QPen erasePen;

        // Type Drawing tools selected in the main window
        DrawToolType currentDrawTool;

        // List of all the shapes drawn by the artist
        QLinkedList<AbstractShape*> shapesList;

        // Timer to prevent the methods refresh() to be called more than 60 times per second
        QTimer* FPSLimiterTimer;

    // Setter
    public :
        void setTools(DrawToolType drawToolType, int penWidth, QColor penBrushColor, QPixmap* cursorPixmap);

    // Constructor
    public :
        Canvas(DrawToolType currentDrawTool, QColor penBrushColor, int penWidth, int width, int height, QWidget* parent);

    // Destructor
    public :
        ~Canvas();

    // Methods
    public :
        // Reset the canvas (make it completly white)
        void reset();

        // When one click of the mouse is pressed
        void mousePressEvent(QMouseEvent* event);
        // When the mouse move and if there is one click pressed
        void mouseMoveEvent(QMouseEvent* event);
        // When the click of the mouse is released
        void mouseReleaseEvent(QMouseEvent* event);

    // Qt slots
    public slots :
        // Refresh the pixmap of the canvas
        void refresh();
};



#endif // CANVAS_H
