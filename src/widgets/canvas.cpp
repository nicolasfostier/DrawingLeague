#include "include/widgets/canvas.h"



// Setter
void Canvas::setTools(DrawingToolType drawingToolType, int penWidth, QColor penBrushColor, QPixmap *cursorPixmap){
    this->currentDrawingTool = drawingToolType;
    this->drawingPen.setWidth(penWidth);
    this->drawingPen.setColor(penBrushColor);
    this->erasePen.setWidth(penWidth);
    this->drawingBrush.setColor(penBrushColor);
    if(cursorPixmap != NULL){
        this->setCursor(QCursor(*cursorPixmap, (cursorPixmap->width() / 2) - 1, (cursorPixmap->width() / 2) - 1));
    }
    else{
        this->setCursor(QCursor());
    }
}
void Canvas::setIsArtist(bool isArtist){
    this->isArtist = isArtist;
}



// Constructor
Canvas::Canvas(DrawingToolType currentDrawingTool, QColor penBrushColor, int penWidth, int width, int height, QWidget* parent) : QLabel(parent)
{
    this->isArtist = true;

    this->currentDrawingTool = currentDrawingTool;
    this->width = width;
    this->height = height;

    // Pixmap of the canvas
    pixmap = new QPixmap(width, height);

    // Painter of the canvas
    painter = new QPainter(pixmap);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // Pen to draw on the canvas
    drawingPen.setColor(penBrushColor);
    drawingPen.setWidth(penWidth);
    drawingPen.setStyle(Qt::SolidLine);
    drawingPen.setCapStyle(Qt::RoundCap);
    drawingPen.setJoinStyle(Qt::RoundJoin);

    // Brush
    drawingBrush.setColor(penBrushColor);
    drawingBrush.setStyle(Qt::SolidPattern);

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
    TickrateLimiterTimer = new QTimer(this);
    TickrateLimiterTimer->setSingleShot(true);
    // 1000 / 6 = 166.67 of tickrate
    TickrateLimiterTimer->setInterval(6);
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
}

//
void Canvas::displayTypeReady(){
    //
    reset();

    //
    pixmap->fill(Qt::black);

    //
    QFont font;
    font.setBold(true);

    // "Type !ready in the chat to start another game"
    font.setPixelSize(24);
    painter->setPen(Qt::white);
    painter->setFont(font);
    painter->drawText(pixmap->rect(), Qt::AlignCenter, tr("Type !ready in the chat,\nif you want to start a game."));

    //
    this->setPixmap(*pixmap);
}

//
void Canvas::displayWinner(QString winner){
    //
    reset();

    //
    pixmap->fill(Qt::black);

    //
    QFont font;
    font.setBold(true);

    QRect rectText;
    rectText.setWidth(pixmap->rect().width());
    rectText.setHeight(pixmap->rect().height());

    // Pseudo of the winner
    font.setPixelSize(75);
    rectText.setTopLeft(QPoint(0, 175));
    painter->setPen(Qt::yellow);
    painter->setFont(font);
    painter->drawText(rectText, Qt::AlignHCenter, winner);

    // "is our great winner !"
    font.setPixelSize(32);
    rectText.setTopLeft(QPoint(0, 280));
    painter->setPen(Qt::white);
    painter->setFont(font);
    painter->drawText(rectText, Qt::AlignHCenter, tr("is our great winner !"));

    // "Type !ready in the chat to start another game"
    font.setPixelSize(24);
    rectText.setTopLeft(QPoint(0, 525));
    painter->setPen(Qt::white);
    painter->setFont(font);
    painter->drawText(rectText, Qt::AlignHCenter, tr("Type !ready in the chat,\nif you want to start another game."));

    //
    this->setPixmap(*pixmap);
}

// When one click of the mouse is pressed
void Canvas::mousePressEvent(QMouseEvent* event){
    if(isArtist || event->screenPos() == QPoint(-232323,-232323)){
        event->accept();

        switch(currentDrawingTool){
            case PEN :{
                painter->setPen(drawingPen);
                painter->drawPoint(event->pos());
            break;
            }

            case ERASER :{
                painter->setPen(erasePen);
                painter->drawPoint(event->pos());
            break;
            }
        }

        lastMousePositionDrawn = event->pos();

        this->setPixmap(*pixmap);

        if(event->screenPos() != QPoint(-232323,-232323)){
            emit mousePressEventToSend(event->pos());
        }
    }
    else{
        event->ignore();
    }
}

// When the click of the mouse is released
void Canvas::mouseReleaseEvent(QMouseEvent* event){
    if(isArtist || event->screenPos() == QPoint(-232323,-232323)){
        event->accept();

        if(event->screenPos() != QPoint(-232323,-232323)){
            emit mouseReleaseEventToSend(event->pos());
        }
    }
    else{
        event->ignore();
    }
}



// Qt slots

// When the mouse move and if there is one click pressed
void Canvas::mouseMoveEvent(QMouseEvent* event){
    if(isArtist || event->screenPos() == QPoint(-232323,-232323)){
        event->accept();

        if(event->screenPos() == QPoint(-232323,-232323)){
            switch(currentDrawingTool){
                case PEN :{
                    painter->drawLine(lastMousePositionDrawn, event->pos());
                break;
                }

                case ERASER :{
                    painter->drawLine(lastMousePositionDrawn, event->pos());
                break;
                }
            }

            lastMousePositionDrawn = event->pos();

            this->setPixmap(*pixmap);
        }
        else{
            //
            if(!TickrateLimiterTimer->isActive()){

                switch(currentDrawingTool){
                    case PEN :{
                        painter->drawLine(lastMousePositionDrawn, event->pos());
                    break;
                    }

                    case ERASER :{
                        painter->drawLine(lastMousePositionDrawn, event->pos());
                    break;
                    }
                }

                lastMousePositionDrawn = event->pos();

                this->setPixmap(*pixmap);

                emit mouseMoveEventToSend(event->pos());

                TickrateLimiterTimer->start();

                // Prevent an infinite loop with our timer
                QObject::disconnect(TickrateLimiterTimer, SIGNAL(timeout()), this, SLOT(mapTimeoutSignal()));
            }
            // If the timer is active we wait for the timer to finished to refresh the canvas
            else{
                lastMousePositionNotDrawn = event->pos();

                QObject::connect(TickrateLimiterTimer, SIGNAL(timeout()), this, SLOT(mapTimeoutSignal()));
            }
        }
    }
    else{
        event->ignore();
    }
}


//
void Canvas::mousePressEventFromServer(QPoint pos){
    QMouseEvent fakeMousePressEvent(QEvent::MouseButtonPress, pos, QPoint(-232323,-232323), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    this->mousePressEvent(&fakeMousePressEvent);
}

//
void Canvas::mouseMoveEventFromServer(QPoint pos){
    QMouseEvent fakeMouseMoveEvent(QEvent::MouseMove, pos, QPoint(-232323,-232323), Qt::NoButton, Qt::LeftButton, Qt::NoModifier);
    this->mouseMoveEvent(&fakeMouseMoveEvent);
}

//
void Canvas::mouseReleaseEventFromServer(QPoint pos){
    QMouseEvent fakeMouseReleaseEvent(QEvent::MouseButtonRelease, pos, QPoint(-232323,-232323), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    this->mouseReleaseEvent(&fakeMouseReleaseEvent);
}


//
void Canvas::mapTimeoutSignal(){
    QMouseEvent fakeMouseMoveEvent(QEvent::MouseMove, lastMousePositionNotDrawn, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mouseMoveEvent(&fakeMouseMoveEvent);
}
