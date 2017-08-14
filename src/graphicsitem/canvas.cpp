#include "include/graphicsitem/canvas.h"



// Setter
void Canvas::setTools(DrawingToolType drawingToolType, int penWidth,
					  QColor penBrushColor, QPixmap *cursorPixmap){
	this->currentDrawingTool = drawingToolType;
	this->drawingPen.setWidth(penWidth);
	this->drawingPen.setColor(penBrushColor);
	this->erasePen.setWidth(penWidth);
	this->drawingBrush.setColor(penBrushColor);
	if(cursorPixmap != NULL){
		if(drawingToolType == BUCKET){
			this->setCursor(bucketCursor);
		}
		else{
			this->setCursor(QCursor(*cursorPixmap,
									(cursorPixmap->width() / 2) - 1,
									(cursorPixmap->width() / 2) - 1));
		}
	}
	else{
		this->setCursor(QCursor());
	}
}
void Canvas::setIsArtist(bool isArtist){
	this->isArtist = isArtist;
}



// Constructor
Canvas::Canvas(DrawingToolType currentDrawingTool, QColor penBrushColor, int penWidth,
			   int width, int height) : QGraphicsItem(){
	this->isArtist = true;

	this->currentDrawingTool = currentDrawingTool;

	image = QImage(width, height, QImage::Format_RGB32);

	painter = new QPainter(&image);

	drawingPen.setColor(penBrushColor);
	drawingPen.setWidth(penWidth);
	drawingPen.setStyle(Qt::SolidLine);
	drawingPen.setCapStyle(Qt::RoundCap);
	drawingPen.setJoinStyle(Qt::RoundJoin);

	drawingBrush.setColor(penBrushColor);
	drawingBrush.setStyle(Qt::SolidPattern);

	erasePen.setColor(Qt::white);
	erasePen.setWidth(penWidth);
	erasePen.setStyle(Qt::SolidLine);
	erasePen.setCapStyle(Qt::RoundCap);
	erasePen.setJoinStyle(Qt::RoundJoin);

	this->reset();

	FPSLimiter = new QTimer(this);
	FPSLimiter->setSingleShot(true);
	// 1000 / 16 = 62.5FPS
	FPSLimiter->setInterval(16);

	TickrateLimiter = new QTimer(this);
	TickrateLimiter->setSingleShot(true);
	// 1000 / 10 = 100 of tickrate
	TickrateLimiter->setInterval(10);

	bucketCursor = QCursor(QIcon(":/images/drawingtools/bucket.ico").pixmap(QSize(24,24)), 2, 18);
}



// Destructor
Canvas::~Canvas(){
	delete painter;
}



// Methods

void Canvas::floodFill(QPoint originalPoint, QColor previousColor, QColor newColor){
	QStack<QPoint> stack;
	if(image.pixelColor(originalPoint) == previousColor
	   && image.pixelColor(originalPoint) != newColor){
		stack.push(originalPoint);

		QPoint currentPoint;
		QPoint west;
		QPoint east;
		QPoint north;
		QPoint south;
		while(!stack.empty()){
			currentPoint = stack.pop();

			if(image.pixelColor(currentPoint) == previousColor){
				west = currentPoint;
				while(image.valid(west) && image.pixelColor(west) == previousColor){
					image.setPixelColor(west, newColor);

					north = west;
					north.setY(north.y() - 1);
					if(image.valid(north) && image.pixelColor(north) == previousColor){
						stack.push(north);
					}

					south = west;
					south.setY(south.y() + 1);
					if(image.valid(south) && image.pixelColor(south) == previousColor){
						stack.push(south);
					}

					west.setX(west.x() - 1);
				}
				east = currentPoint;
				east.setX(east.x() + 1);
				while(image.valid(east) && image.pixelColor(east) == previousColor){
					image.setPixelColor(east, newColor);

					north = east;
					north.setY(north.y() - 1);
					if(image.valid(north) && image.pixelColor(north) == previousColor){
						stack.push(north);
					}

					south = east;
					south.setY(south.y() + 1);
					if(image.valid(south) && image.pixelColor(south) == previousColor){
						stack.push(south);
					}

					east.setX(east.x() + 1);
				}
			}
		}
	}
}

void Canvas::reset(){
	image.fill(Qt::white);
	this->update();
}


void Canvas::mousePressEvent(QGraphicsSceneMouseEvent * event){
	if(isArtist || event->screenPos() == QPointF(-232323,-232323)){
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


			case BUCKET:{

			break;
			}
		}

		lastMousePositionDrawn = event->pos();

		this->refresh();

		if(event->screenPos() != QPointF(-232323,-232323)){
			emit mousePressEventToSend(event->pos().toPoint());
		}
	}
	else{
		event->ignore();
	}
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
	if(isArtist || event->screenPos() == QPointF(-232323,-232323)){
		event->accept();

		switch(currentDrawingTool){
			case PEN :{

			break;
			}

			case ERASER:{

			break;
			}

			case BUCKET :{
				floodFill(event->pos().toPoint(), image.pixelColor(event->pos().toPoint()), drawingPen.color());
			break;
			}
		}

		this->refresh();

		if(event->screenPos() != QPointF(-232323,-232323)){
			emit mouseReleaseEventToSend(event->pos().toPoint());
		}
	}
	else{
		event->ignore();
	}
}


QRectF Canvas::boundingRect() const{
	return image.rect();
}

void Canvas::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(widget);
	Q_UNUSED(option);

	painter->drawImage(0, 0, image);
}



// Qt slots

void Canvas::mapTickrateTimeoutSignal(){
	QGraphicsSceneMouseEvent fakeMouseMoveEvent(QEvent::GraphicsSceneMouseMove);
	fakeMouseMoveEvent.setPos(lastMousePositionNotDrawn);
	fakeMouseMoveEvent.setScenePos(QPointF(-232323,-232323));
	fakeMouseMoveEvent.setButton(Qt::NoButton);
	fakeMouseMoveEvent.setButtons(Qt::LeftButton);
	fakeMouseMoveEvent.setModifiers(Qt::NoModifier);

	mouseMoveEvent(&fakeMouseMoveEvent);
}

void Canvas::refresh(){
	if(!FPSLimiter->isActive()){
		this->update();
		QObject::disconnect(FPSLimiter, SIGNAL(timeout()),
							this, SLOT(refresh()));
	}
	else{
		QObject::connect(FPSLimiter, SIGNAL(timeout()),
						 this, SLOT(refresh()));
	}
}


void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent * event){
	if(isArtist || event->screenPos() == QPointF(-232323,-232323)){
		event->accept();

		if(!TickrateLimiter->isActive() || event->screenPos() == QPointF(-232323,-232323)){
			switch(currentDrawingTool){
				case PEN :{
					painter->drawLine(lastMousePositionDrawn, event->pos());
				break;
				}

				case ERASER:{
					painter->drawLine(lastMousePositionDrawn, event->pos());
				break;
				}

				case BUCKET :{

				break;
				}
			}

			lastMousePositionDrawn = event->pos();

			this->refresh();

			if(!TickrateLimiter->isActive()){
				emit mouseMoveEventToSend(event->pos().toPoint());

				TickrateLimiter->start();

				// Prevent an infinite loop with our timer
				disconnect(TickrateLimiter, SIGNAL(timeout()),
							this, SLOT(mapTickrateTimeoutSignal()));
			}
		}
		else{
			lastMousePositionNotDrawn = event->pos();

			QObject::connect(TickrateLimiter, SIGNAL(timeout()),
							 this, SLOT(mapTickrateTimeoutSignal()));
		}
	}
	else{
		event->ignore();
	}
}


void Canvas::mousePressEventFromServer(QPoint pos){
	QGraphicsSceneMouseEvent fakeMousePressEvent(QEvent::GraphicsSceneMousePress);
	fakeMousePressEvent.setPos(QPointF(pos));
	fakeMousePressEvent.setScreenPos(QPoint(-232323,-232323));
	fakeMousePressEvent.setButton(Qt::LeftButton);
	fakeMousePressEvent.setButtons(Qt::LeftButton);
	fakeMousePressEvent.setModifiers(Qt::NoModifier);

	this->mousePressEvent(&fakeMousePressEvent);
}

void Canvas::mouseMoveEventFromServer(QPoint pos){
	QGraphicsSceneMouseEvent fakeMouseMoveEvent(QEvent::GraphicsSceneMouseMove);
	fakeMouseMoveEvent.setPos(QPointF(pos));
	fakeMouseMoveEvent.setScreenPos(QPoint(-232323,-232323));
	fakeMouseMoveEvent.setButton(Qt::NoButton);
	fakeMouseMoveEvent.setButtons(Qt::LeftButton);
	fakeMouseMoveEvent.setModifiers(Qt::NoModifier);

	this->mouseMoveEvent(&fakeMouseMoveEvent);
}

void Canvas::mouseReleaseEventFromServer(QPoint pos){
	QGraphicsSceneMouseEvent fakeMouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
	fakeMouseReleaseEvent.setPos(QPointF(pos));
	fakeMouseReleaseEvent.setScreenPos(QPoint(-232323,-232323));
	fakeMouseReleaseEvent.setButton(Qt::LeftButton);
	fakeMouseReleaseEvent.setButtons(Qt::NoButton);
	fakeMouseReleaseEvent.setModifiers(Qt::NoModifier);

	this->mouseReleaseEvent(&fakeMouseReleaseEvent);
}
