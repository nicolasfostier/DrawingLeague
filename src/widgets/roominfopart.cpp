#include "include/widgets/roominfopart.h"


// Setters
void RoomInfoPart::setValue(QString value){
	this->value->setText(value);
}



// Getters
QString RoomInfoPart::getValue(){
	return value->text();
}



// Constructor
RoomInfoPart::RoomInfoPart(QString label, QString value, QWidget* parent) : QWidget(parent)
{
	layout = new QHBoxLayout(this);

		label.append(" :");
		this->label = new QLabel(label, this);
		layout->addWidget(this->label);

			labelFont.setBold(true);
			this->label->setFont(labelFont);

		this->value = new QLabel(value, this);
		layout->addWidget(this->value);
}
