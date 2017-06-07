#ifndef PLAYER_H
#define PLAYER_H



#include <QTableWidgetItem>



//
class Player
{
    // Variables
    private :
        //
        QTableWidgetItem* pseudo;
        //
        QTableWidgetItem* score;
        //
        bool isReady;
        //
        bool answerFound;

        //
        QFont hasntFoundFont;
        //
        QFont hasFoundFont;
        //
        QFont isArtistFont;

    // Getter
    public :
        QString getPseudo();
        int getScore();
        bool getIsReady();
        bool getAnswerFound();

    // Setter
    public :
        void setScore(int score);
        void setIsReady(bool isReady);
        void setAnswerFound(bool answerFound);

    // Constructor
    public :
        Player(QString pseudo = " ", int score = 0);
        Player(const Player& player);

    // Destructor
    public :
        ~Player();

    // Methods
    public :
        //
        void addToTableWidget(QTableWidget* playersTable);
        //
        void hasntFound();
        //
        void hasFound();
        //
        void isArtist();


    // Operators
    public :
        friend QDataStream& operator<<(QDataStream& dataStream, Player player);
        friend QDataStream& operator>>(QDataStream& dataStream, Player& player);
};

Q_DECLARE_METATYPE(Player)

//
QDataStream& operator<<(QDataStream& dataStream, Player player);
//
QDataStream& operator>>(QDataStream& dataStream, Player& player);



#endif // PLAYER_H
