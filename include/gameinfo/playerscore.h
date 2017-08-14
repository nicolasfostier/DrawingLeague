#ifndef PLAYERSCORE_H
#define PLAYERSCORE_H



#include <QTableWidgetItem>



// Handle the score of a player
/* It's needed to overload the comparison operator :
 * we can't sort the player by score without it */
class PlayerScore : public QTableWidgetItem{
	// Constructeur
	public :
		PlayerScore(QString score);

	// Operator
	public :
		bool operator<(const QTableWidgetItem &other) const;
};



#endif // PLAYERSCORE_H
