#include "include/widgets/roominfopart.h"


// Setters
void RoomInfoPart::setValue(QString value){
    this->value->setText(value);
}
void RoomInfoPart::setValueFont(QFont font){
    this->value->setFont(font);
}
void RoomInfoPart::setValueColor(QColor color){
    QPalette newPalette(this->value->palette());
    newPalette.setColor(QPalette::Active, QPalette::WindowText, color);
    newPalette.setColor(QPalette::Inactive, QPalette::WindowText, color);
    this->value->setPalette(newPalette);
}


// Constructor
RoomInfoPart::RoomInfoPart(QString label, QString value, QWidget* parent) : QWidget(parent)
{
    //
    layout = new QHBoxLayout(this);

        //
        label.append(" :");
        this->label = new QLabel(label, this);
        layout->addWidget(this->label);

            //
            labelFont.setBold(true);
            this->label->setFont(labelFont);

        this->value = new QLabel(value, this);
        layout->addWidget(this->value);
}
