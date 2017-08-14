#ifndef PLAYER_H
#define PLAYER_H



#include <QTableWidgetItem>


#include "include/gameinfo/playerscore.h"



// Handle informations about a player
class Player
{
	// Variables
	private :
		QTableWidgetItem* pseudo;
		PlayerScore* score;

		bool isReady;
		bool isArtist;
		bool answerFound;

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
		void pseudoToHTMLEscaped();
		void addToTableWidget(QTableWidget* playersTable);
		void updateColor();


	// Operators
	public :
		friend QDataStream& operator<<(QDataStream& dataStream, Player player);
		friend QDataStream& operator>>(QDataStream& dataStream, Player& player);
};

Q_DECLARE_METATYPE(Player)

QDataStream& operator<<(QDataStream& dataStream, Player player);
QDataStream& operator>>(QDataStream& dataStream, Player& player);



#endif // PLAYER_H
