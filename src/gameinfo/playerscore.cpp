#include "include/gameinfo/playerscore.h"



// Constructeur
PlayerScore::PlayerScore(QString score) : QTableWidgetItem(score)
{

}



// Operator
bool PlayerScore::operator<(const QTableWidgetItem &other) const{
	return text().toInt() < other.text().toInt();
}
