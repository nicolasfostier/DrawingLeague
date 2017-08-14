#include "include/widgets/canvasview.h"



// Getter
Canvas* CanvasView::getCanvas(){
	return canvas;
}
ReadyButton* CanvasView::getReadyButton(){
	return readyButton;
}



// Constructor
CanvasView::CanvasView(QRectF bounds, QWidget* parent) : QGraphicsView(parent){
	this->bounds = bounds.marginsRemoved(QMarginsF(5,5,5,5));;

	this->setRenderHints(QPainter::Antialiasing |QPainter::SmoothPixmapTransform);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setMinimumSize(600,600);
	QSizePolicy sizePolicy;
	sizePolicy.setHeightForWidth(true);
	sizePolicy.setWidthForHeight(true);
	this->setSizePolicy(sizePolicy);

	canvasScene = new QGraphicsScene(this);
	canvasScene->setSceneRect(0,0,1000,1000);

		canvas = new Canvas(PEN, Qt::red, 3, 1000, 1000);
		canvasScene->addItem(canvas);

	gameNotStartedScene = new QGraphicsScene(this);
	gameNotStartedScene->setSceneRect(0,0,1000,1000);
	QPixmap black(1000,1000);
	black.fill(Qt::black);
	gameNotStartedScene->addPixmap(black);

	QTextOption textOption;
	textOption.setAlignment(Qt::AlignCenter);

	endGameScene = new QGraphicsScene(this);
	endGameScene->setSceneRect(0,0,1000,1000);
	endGameScene->addPixmap(black);

		winnerPseudo = new QGraphicsTextItem();
		winnerPseudo->document()->setDefaultTextOption(textOption);
		winnerPseudo->setTextWidth(800);
		winnerPseudo->setDefaultTextColor(Qt::yellow);
			QFont font;
			font.setPixelSize(100);
			font.setBold(true);
			winnerPseudo->setFont(font);
		endGameScene->addItem(winnerPseudo);

		winnerText = new QGraphicsTextItem(tr("is our great winner !"));
		winnerText->document()->setDefaultTextOption(textOption);
		winnerText->setTextWidth(800);
		winnerText->setDefaultTextColor(QColor(Qt::white));
			font.setPixelSize(50);
			font.setBold(true);
		winnerText->setFont(font);
			QPointF winnerTextPos = endGameScene->sceneRect().center();
			winnerTextPos.setX(winnerTextPos.x() - (winnerText->boundingRect().width() / 2.0));
			winnerTextPos.setY(winnerTextPos.y() - (winnerText->boundingRect().height() / 2.0));
		winnerText->setPos(winnerTextPos);
		endGameScene->addItem(winnerText);

	readyNeeded = new QGraphicsTextItem();
	readyNeeded->document()->setDefaultTextOption(textOption);
	readyNeeded->setTextWidth(800);
	readyNeeded->setDefaultTextColor(Qt::white);
		font.setPixelSize(30);
	readyNeeded->setFont(font);

	readyButton = new ReadyButton();

		readyButtonPosGNS = gameNotStartedScene->sceneRect().center();
		readyButtonPosGNS.setX(readyButtonPosGNS.x() - (readyButton->boundingRect().width() / 2.0));
		readyButtonPosGNS.setY(readyButtonPosGNS.y() - (readyButton->boundingRect().height() / 2.0));

		readyButtonPosEG = readyButtonPosGNS;
		readyButtonPosEG.setY(800);

	displayCanvas();

	this->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
}



// Destructor
CanvasView::~CanvasView(){
	delete canvas;
	delete winnerPseudo;
	delete winnerText;
	delete readyNeeded;
	delete readyButton;
}


// Methods

void CanvasView::displayCanvas(){
	this->setScene(canvasScene);
}

void CanvasView::displayGameNotStarted(){
	this->readyButton->setPos(readyButtonPosGNS);
	this->readyButton->setIsReady(false);

	this->gameNotStartedScene->addItem(readyNeeded);
	this->gameNotStartedScene->addItem(readyButton);

	this->setScene(gameNotStartedScene);

	updateReadyNeeded(99);
}

void CanvasView::displayEndGame(QString winner){
	winnerPseudo->setPlainText(winner);
	QPointF winnerPseudoPos = endGameScene->sceneRect().center();
	winnerPseudoPos.setX(winnerPseudoPos.x() - (winnerPseudo->boundingRect().width() / 2.0));
	winnerPseudoPos.setY(350);
	winnerPseudo->setPos(winnerPseudoPos);

	this->readyButton->setPos(readyButtonPosEG);
	this->readyButton->setIsReady(false);

	this->endGameScene->addItem(readyNeeded);
	this->endGameScene->addItem(readyButton);

	this->setScene(endGameScene);

	updateReadyNeeded(99);
}


int CanvasView::heightForWidth(int w) const{
	return w;
}

void CanvasView::resizeEvent(QResizeEvent* event){
	event->accept();

	this->fit();
}



// Qt slots

void CanvasView::updateReadyNeeded(int nbReadyNeeded){
	QPointF pos = endGameScene->sceneRect().center();;
	if(this->scene() == gameNotStartedScene){
		readyNeeded->setPlainText(QString::number(nbReadyNeeded) + " " + tr("more ready player(s) are needed to start the game."));
		pos.setX(pos.x() - (readyNeeded->boundingRect().width() / 2.0));
		pos.setY(325);
	}
	else{
		readyNeeded->setPlainText(QString::number(nbReadyNeeded) + " " + tr("more ready player(s) are needed to start a new game."));
		pos.setX(pos.x() - (readyNeeded->boundingRect().width() / 2.0));
		pos.setY(700);
	}
	readyNeeded->setPos(pos);
}


void CanvasView::fit(){
	this->fitInView(bounds, Qt::KeepAspectRatio);
}
