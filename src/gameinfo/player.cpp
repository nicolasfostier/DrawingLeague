#include "include/gameinfo/player.h"



// Getter
QString Player::getPseudo(){
    return pseudo->text();
}
int Player::getScore(){
    return score->text().toInt();
}
bool Player::getIsReady(){
    return isReady;
}
bool Player::getAnswerFound(){
    return answerFound;
}



// Setter
void Player::setScore(int score){
    this->score->setText(QString::number(score));
}
void Player::setIsReady(bool isReady){
    this->isReady = isReady;
}
void Player::setAnswerFound(bool answerFound){
    this->answerFound = answerFound;
}



// Constructor
Player::Player(QString pseudo, int score){
    this->pseudo = new QTableWidgetItem(pseudo);
    this->pseudo->setFlags(Qt::ItemIsEnabled);

    this->score = new QTableWidgetItem(QString::number(score));
    this->score->setFlags(Qt::ItemIsEnabled);

    this->isReady = false;

    this->answerFound = true;

    hasFoundFont.setBold(true);
    isArtistFont.setBold(true);
}
Player::Player(const Player &player){
    this->pseudo = new QTableWidgetItem(*player.pseudo);
    this->pseudo->setFlags(Qt::ItemIsEnabled);

    this->score = new QTableWidgetItem(*player.score);
    this->score->setFlags(Qt::ItemIsEnabled);

    this->isReady = player.isReady;

    this->answerFound = player.answerFound;
}


// Destructor
Player::~Player(){
    delete pseudo;
    delete score;
}


// Methods

//
void Player::addToTableWidget(QTableWidget *playersTable){
    playersTable->setRowCount(playersTable->rowCount() + 1);
    playersTable->setItem(playersTable->rowCount()-1, 0, this->pseudo);
    playersTable->setItem(playersTable->rowCount()-1, 1, this->score);
    playersTable->sortItems(0, Qt::AscendingOrder);
    playersTable->sortItems(1, Qt::DescendingOrder);
}

//
void Player::hasntFound(){
    this->pseudo->setBackgroundColor(QColor(Qt::white));
    this->score->setBackgroundColor(QColor(Qt::white));
    this->pseudo->setFont(hasntFoundFont);
}

//
void Player::hasFound(){
    this->pseudo->setBackgroundColor(QColor("#de4d4d"));
    this->score->setBackgroundColor(QColor("#de4d4d"));
    this->pseudo->setFont(hasFoundFont);
}

//
void Player::isArtist(){
    this->pseudo->setBackgroundColor(QColor("#87d687"));
    this->score->setBackgroundColor(QColor("#87d687"));
    this->pseudo->setFont(isArtistFont);
}



// Operators
QDataStream& operator<<(QDataStream& dataStream, Player player){
    dataStream << *(player.pseudo);
    dataStream << *(player.score);
    dataStream << player.isReady;
    dataStream << player.answerFound;

    return dataStream;
}
QDataStream& operator>>(QDataStream& dataStream, Player& player){
    dataStream >> *(player.pseudo);
    dataStream >> *(player.score);
    dataStream >> player.isReady;
    dataStream >> player.answerFound;

    return dataStream;
}
