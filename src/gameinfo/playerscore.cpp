#include "include/gameinfo/playerscore.h"



// Constructeur
PlayerScoreTWI::PlayerScoreTWI(QString score) : QTableWidgetItem(score)
{

}

//
bool PlayerScoreTWI::operator<(const QTableWidgetItem &other) const{
    return text().toInt() < other.text().toInt();
}
