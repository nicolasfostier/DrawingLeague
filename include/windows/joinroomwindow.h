#ifndef JOINROOMWINDOW_H
#define JOINROOMWINDOW_H



#include <QDialog>
#include <QTranslator>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDataStream>


//
#include "include/datablock.h"



class JoinRoomWindow : public QDialog
{
    Q_OBJECT

    // Variables
    private :
        //
        QTcpSocket* socket;

        //
        QGridLayout* layout;

        //
        QLabel* pseudoLabel;
        //
        QLineEdit* pseudoLineEdit;

        //
        QLabel* ipLabel;
        //
        QLineEdit* ipLineEdit;

        //
        QLabel* portLabel;
        //
        QLineEdit* portLineEdit;

        //
        QWidget* connectOrCancel;
            //
            QHBoxLayout* layoutConnectOrCancel;
                //
                QPushButton* cancelButton;
                //
                QPushButton* connectButton;

    // Constructor
    public :
        JoinRoomWindow(QTcpSocket* socket);

    // Qt slots
    public slots:
        //
        void connectToTheServeur();
        //
        void connectOk();
        //
        void connectError();
};



#endif // JOINROOMWINDOW_H
