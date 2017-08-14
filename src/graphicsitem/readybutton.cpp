#include "include/graphicsitem/readybutton.h"



// Setter
void ReadyButton::setIsReady(bool isReady){
	this->isReady = isReady;
	if(!isReady){
		this->setCursor(QCursor(Qt::PointingHandCursor));
	}
}



// Constructeur
ReadyButton::ReadyButton(){
	isMouseHover = false;
	isMousePressed = false;

	isReady = false;
	this->setCursor(QCursor(Qt::PointingHandCursor));
	rect = QRectF(0,0,600,150);

	brush = QBrush(Qt::SolidPattern);

	setAcceptHoverEvents(true);
}



// Destructor
ReadyButton::~ReadyButton(){

}



// Methods

void ReadyButton::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
	if(!isReady){
		event->accept();

		isMouseHover = true;

		this->update();
	}
	else{
		event->ignore();
	}
}

void ReadyButton::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
	if(!isReady){
		event->accept();

		isMouseHover = false;
		isMousePressed = false;

		this->update();
	}
	else{
		event->ignore();
	}
}


void ReadyButton::mousePressEvent(QGraphicsSceneMouseEvent* event){
	if(!isReady){
		event->accept();

		isMousePressed = true;

		this->update();
	}
	else{
		event->ignore();
	}
}

void ReadyButton::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
	if(!isReady){
		event->accept();

		isMousePressed = false;
		isMouseHover = false;

		if(this->boundingRect().contains(event->pos())){
			this->setCursor(QCursor(Qt::ArrowCursor));

			isReady = true;
			emit ready();
		}

		this->update();
	}
	else{
		event->ignore();
	}
}


QRectF ReadyButton::boundingRect() const{
	return rect;
}

void ReadyButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option);
	Q_UNUSED(widget);

	pen.setColor(Qt::gray);
	pen.setWidth(3);
	painter->setPen(pen);

	int fontSize = 64;
	QString text;

	if(!isReady){
		text = tr("Ready ?");

		if(isMouseHover){
			brush.setColor("#ec2b2b");

			if(isMousePressed){
				rect.marginsRemoved(QMargins(30,15,30,15));
				fontSize = 60;
			}
		}
		else{
			brush.setColor("#dd1212");
		}
	}
	else{
		text = tr("Ready !");

		brush.setColor("#33c633");
	}

	painter->setBrush(brush);
	painter->drawRoundedRect(rect, 40, 40);

	pen.setColor(Qt::white);
	painter->setPen(pen);
	QTextOption textOption(Qt::AlignCenter);
	QFont font;
	font.setBold(true);
	font.setPixelSize(fontSize);
	painter->setFont(font);
	painter->drawText(rect, text, textOption);
}
