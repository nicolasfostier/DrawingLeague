#include "include/widgets/roominfopart.h"


// Setters
void RoomInfoPart::setValue(QString value){
    this->value->setText(value);
}


// Constructor
RoomInfoPart::RoomInfoPart(QString label, QString value, QWidget* parent) : QWidget(parent)
{
    //
    layout = new QHBoxLayout(this);

        //
        label = "<b>" + label + " :</b>";
        this->label = new QLabel(label, this);
        layout->addWidget(this->label);

        this->value = new QLabel(value, this);
        layout->addWidget(this->value);
}
