#include "include/widgets/canvas.h"



// Setter
void Canvas::setTools(DrawToolType drawToolType, int penWidth, QColor penBrushColor, QPixmap *cursorPixmap){
    this->currentDrawTool = drawToolType;
    this->drawPen.setWidth(penWidth);
    this->drawPen.setColor(penBrushColor);
    this->erasePen.setWidth(penWidth);
    this->drawBrush.setColor(penBrushColor);
    this->setCursor(QCursor(*cursorPixmap, (cursorPixmap->width() / 2) - 1, (cursorPixmap->width() / 2) - 1));
}



// Constructor
Canvas::Canvas(DrawToolType currentDrawTool, QColor penBrushColor, int penWidth, int width, int height, QWidget* parent) : QLabel(parent)
{
    this->currentDrawTool = currentDrawTool;
    this->width = width;
    this->height = height;

    // Pixmap of the canvas
    pixmap = new QPixmap(width, height);

    // Painter of the canvas
    painter = new QPainter(pixmap);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // Pen to draw on the canvas
    drawPen.setColor(penBrushColor);
    drawPen.setWidth(penWidth);
    drawPen.setStyle(Qt::SolidLine);
    drawPen.setCapStyle(Qt::RoundCap);
    drawPen.setJoinStyle(Qt::RoundJoin);

    // Brush
    drawBrush.setColor(penBrushColor);
    drawBrush.setStyle(Qt::SolidPattern);

    // Pen to erase (just a white pen)
    erasePen.setColor(Qt::white);
    erasePen.setWidth(penWidth);
    erasePen.setStyle(Qt::SolidLine);
    erasePen.setCapStyle(Qt::RoundCap);
    erasePen.setJoinStyle(Qt::RoundJoin);

    // Paint the canvas in white
    this->reset();

    // Display the canvas
    this->setPixmap(*pixmap);

    // Add a gray border around the canvas
    this->setStyleSheet("border: 1px solid grey");

    // Timer to prevent the methods refresh() to be called more than 60 times per second
    FPSLimiterTimer = new QTimer(this);
    FPSLimiterTimer->setSingleShot(true);
}



// Destructor
Canvas::~Canvas(){
    delete painter;
    delete pixmap;
}



// Methods

// Reset the canvas (make it completly white)
void Canvas::reset(){
    pixmap->fill(Qt::white);
    this->setPixmap(*pixmap);

    shapesList.clear();
}

// When one click of the mouse is pressed
void Canvas::mousePressEvent(QMouseEvent* event){
    event->accept();

    switch(currentDrawTool){
        case PEN :{
            Polyline* newPolyline = new Polyline(drawPen, event->pos());
            shapesList.append(newPolyline);

            painter->setPen(drawPen);
            painter->drawPoint(event->pos());

            this->refresh();
        break;
        }

        case ERASER :{
            Polyline* newPolyline = new Polyline(drawPen, event->pos());
            shapesList.append(newPolyline);

            painter->setPen(erasePen);
            painter->drawPoint(event->pos());

            this->refresh();
        break;
        }
    }
}

// When the mouse move and if there is one click pressed
void Canvas::mouseMoveEvent(QMouseEvent* event){
    event->accept();

    switch(currentDrawTool){
        case PEN :{
            Polyline* currentPolyline = static_cast<Polyline*>(shapesList.last());

            painter->drawLine(currentPolyline->getLastPoint(), event->pos());
            this->refresh();

            currentPolyline->addPoint(event->pos());
        break;
        }

        case ERASER :{
            Polyline* currentPolyline = static_cast<Polyline*>(shapesList.last());

            painter->drawLine(currentPolyline->getLastPoint(), event->pos());
            this->refresh();

            currentPolyline->addPoint(event->pos());
        break;
        }
    }
}

// When the click of the mouse is released
void Canvas::mouseReleaseEvent(QMouseEvent* event){
   event->accept();

//   switch(currentDrawTool){
//   }
}



// Qt slots

// Refresh the pixmap of the canvas
void Canvas::refresh(){
    // If the timer isn't active, we refresh the canvas
    if(!FPSLimiterTimer->isActive()){
        this->setPixmap(*pixmap);

        // 1000 / 6 = 166.67FPS
        FPSLimiterTimer->start(6);

        // When the timer is over, we don't necessarily need to refresh the canvas
        // (prevent an infinite loop with our timer which would want to refresh the canvas even if there isn't new stuff to show)
        QObject::disconnect(FPSLimiterTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    }
    // If the timer is active we wait for the timer to finished to refresh the canvas
    else{
        QObject::connect(FPSLimiterTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    }
}
