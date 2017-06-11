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
        bool isArtist;

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
        bool getIsArtist();

    // Setter
    public :
        void setScore(int score);
        void setIsReady(bool isReady);
        void setAnswerFound(bool answerFound);
        void setIsArtist(bool isArtist);

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
        void updateColor();


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
