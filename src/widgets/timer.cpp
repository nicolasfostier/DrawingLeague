#include "include/widgets/timer.h"



// Setter
void Timer::setTimeByRound(int timeByRound){
	this->timeByRound = timeByRound;
}
void Timer::setTimeAfterFirstGoodAnswer(int timeAfterFirstGoodAnswer){
	this->timeAfterFirstGoodAnswer = timeAfterFirstGoodAnswer;
}



// Constructeur
Timer::Timer(int timeByRound, int timeAfterFirstGoodAnswer) : QWidget(){
	qTimer = new QTimer(this);

	FPSLimiter = new QTimer(this);
	FPSLimiter->setSingleShot(true);
	FPSLimiter->setInterval(33);
	QObject::connect(FPSLimiter, SIGNAL(timeout()),
					 this, SLOT(update()));

	layout = new QHBoxLayout(this);
	layout->setContentsMargins(4,4,4,4);

		QString label = tr("Time");
		label.append(" :");
		this->label = new QLabel(label, this);
		layout->addWidget(this->label);

			labelFont.setBold(true);
			this->label->setFont(labelFont);

		this->timerText = new QLabel();
		this->timerText->setContentsMargins(6,0,6,0);
		layout->addWidget(this->timerText);

			timerTextFont.setBold(true);
			timerTextFont.setPixelSize(18);
			timerText->setFont(timerTextFont);
			timerTextPalette = this->timerText->palette();

		this->timerCircle = new QLabel();
		layout->addWidget(this->timerCircle);

			this->timeByRound = timeByRound;
			this->timeAfterFirstGoodAnswer = timeAfterFirstGoodAnswer;
			this->oneHasFound = false;

			pixmap = new QPixmap(50, 50);
			pixmap->fill(Qt::transparent);

			painter = new QPainter(pixmap);
			painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

			pen = new QPen();
			pen->setWidth(1);
			pen->setColor(QColor("#7d7d7d"));

			brush = new QBrush();
			brush->setColor("#f7e219");
			brush->setStyle(Qt::SolidPattern);

	this->update();
}



// Destructeur
Timer::~Timer(){
	delete painter;
	delete pixmap;
	delete pen;
	delete brush;
}



// Methods

void Timer::start(int timeMs, bool oneHasFound){
	this->oneHasFound = oneHasFound;

	qTimer->setInterval(timeMs);
	qTimer->start();

	this->update();
}

void Timer::stop(){
	this->oneHasFound = false;
	qTimer->stop();

	this->update();
}

void Timer::update(){
	this->timerText->setText(QString::number((qTimer->remainingTime() - 1) / 60000) + ":"
							 + QString("%1").arg(((qTimer->remainingTime() - 1) / 1000) % 60, 2, 10, QChar('0')));
	if(oneHasFound){
		timerTextPalette.setColor(QPalette::Active, QPalette::WindowText, Qt::red);
		timerTextPalette.setColor(QPalette::Inactive, QPalette::WindowText, Qt::red);
		this->timerText->setPalette(timerTextPalette);

		brush->setColor("#f92e2e");
	}
	else{
		timerTextPalette.setColor(QPalette::Active, QPalette::WindowText, Qt::black);
		timerTextPalette.setColor(QPalette::Inactive, QPalette::WindowText, Qt::black);
		this->timerText->setPalette(timerTextPalette);

		brush->setColor("#f7e219");
	}



	pixmap->fill(Qt::transparent);

	int angle;
	if(!qTimer->isActive()){
		angle = 0;
	}
	else if(oneHasFound){
		angle = static_cast<float>(qTimer->remainingTime() - 1000) / (timeAfterFirstGoodAnswer * 1000) * 5760;
	}
	else{
		angle = static_cast<float>(qTimer->remainingTime() - 1000) / (timeByRound * 1000) * 5760;
	}

	painter->setPen(Qt::NoPen);
	painter->setBrush(*brush);
	painter->drawPie(1, 1, 48, 48, 1440, -1 * angle);

	painter->setPen(*pen);
	painter->setBrush(Qt::NoBrush);
	painter->drawEllipse(1, 1, 48, 48);

	this->timerCircle->setPixmap(*pixmap);

	if(qTimer->remainingTime() > 1000){
		FPSLimiter->start();
	}
	else{
		pixmap->fill(Qt::transparent);

		painter->setPen(Qt::NoPen);
		painter->setBrush(*brush);
		painter->drawPie(1, 1, 48, 48, 1440, 0);

		painter->setPen(*pen);
		painter->setBrush(Qt::NoBrush);
		painter->drawEllipse(1, 1, 48, 48);

		this->timerCircle->setPixmap(*pixmap);
	}
}
