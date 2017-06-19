#ifndef PLAYERSCORE_H
#define PLAYERSCORE_H



#include <QTableWidgetItem>



//
class PlayerScoreTWI : public QTableWidgetItem{
    // Constructeur
    public :
        PlayerScoreTWI(QString score);

    // Operators
    public :
        bool operator<(const QTableWidgetItem &other) const;
};



#endif // PLAYERSCORE_H
