#ifndef ROOMINFOPART_H
#define ROOMINFOPART_H



#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>



// Part of the RoomInfo class,
// which display only one distinct information about the room
class RoomInfoPart : public QWidget
{
	// Variables
	private :
		QHBoxLayout* layout;
			QLabel* label;
				QFont labelFont;
			QLabel* value;

	// Setters
	public :
		void setValue(QString value);

	// Getters
	public :
		QString getValue();

	// Constructor
	public:
		RoomInfoPart(QString label, QString value, QWidget* parent = 0);
};



#endif // ROOMINFOPART_H
