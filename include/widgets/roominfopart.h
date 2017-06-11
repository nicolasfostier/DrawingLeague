#ifndef ROOMINFOPART_H
#define ROOMINFOPART_H



#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>



//
class RoomInfoPart : public QWidget
{
    // Variables
    private :
        //
        QHBoxLayout* layout;
            //
            QLabel* label;
                //
                QFont labelFont;
            //
            QLabel* value;

    // Setters
    public :
        void setValue(QString value);
        void setValueFont(QFont font);
        void setValueColor(QColor color);

    // Constructor
    public:
        RoomInfoPart(QString label, QString value, QWidget* parent = 0);
};



#endif // ROOMINFOPART_H
