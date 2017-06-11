#ifndef CANVAS_H
#define CANVAS_H



#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QLinkedList>
#include <QCursor>
#include <QTimer>
#include <QTextOption>
#include <QImage>
#include <QStack>
#include <QIcon>


// A polyline which can be drawn by the artist
#include "include/shapes/polyline.h"
// All the type of tool to draw on the canvas
#include "include/drawingtooltype.h"
//
#include "include/datablock/datablockreader.h"
//
#include "include/datablock/datablockwriter.h"



// Canvas where the artist can draw
class Canvas : public QLabel
{
    Q_OBJECT

    // Variables
    private :
        bool isArtist;

        // Width of the canvas
        int width;
        // Height of the canvas
        int height;

        // Pixmap of the canvas
        QPixmap pixmap;
            // Image version of the pixmap
            QImage image;
        // Painter of the canvas
        QPainter* painter;
        // Pen to draw on the canvas
        QPen drawingPen;
        // Brush to draw on the canvas (for filled stuff)
        QBrush drawingBrush;
        // Pen to erase (just a white pen)
        QPen erasePen;

        // Type Drawing tools selected in the main window
        DrawingToolType currentDrawingTool;

        // Last mouse position registered and drawn
        QPoint lastMousePositionDrawn;
        //
        QPoint lastMousePositionNotDrawn;

        //
        QTimer* TickrateLimiterTimer;

        //
        QCursor bucketCursor;

    // Setter
    public :
        void setTools(DrawingToolType drawingToolType, int penWidth, QColor penBrushColor, QPixmap* cursorPixmap);
        void setIsArtist(bool isArtist);

    // Constructor
    public :
        Canvas(DrawingToolType currentDrawingTool, QColor penBrushColor, int penWidth, int width, int height, QWidget* parent);

    // Destructor
    public :
        ~Canvas();

    // Methods
    public :
        //
        void floodFill(QPoint point, QColor previousColor, QColor newColor);
        // Reset the canvas (make it completly white)
        void reset();

        //
        void displayTypeReady();
        //
        void displayWinner(QString winner);

        // When one click of the mouse is pressed
        void mousePressEvent(QMouseEvent* event);
        // When the click of the mouse is released
        void mouseReleaseEvent(QMouseEvent* event);

    // Qt slots
    public slots :
        // When the mouse move and if there is one click pressed
        void mouseMoveEvent(QMouseEvent* event);

        //
        void mousePressEventFromServer(QPoint pos);
        //
        void mouseMoveEventFromServer(QPoint pos);
        //
        void mouseReleaseEventFromServer(QPoint pos);

        //
        void mapTimeoutSignal();

    // Qt signals
    signals :
        //
        void mousePressEventToSend(QPoint pos);
        //
        void mouseMoveEventToSend(QPoint pos);
        //
        void mouseReleaseEventToSend(QPoint pos);
};



#endif // CANVAS_H
