#ifndef CREATEROOMWINDOW_H
#define CREATEROOMWINDOW_H



#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>


//
#include "include/server.h"



class CreateRoomWindow : public QDialog
{
    Q_OBJECT

    // Variables
    private :
        //
        Server* server;
        //
        QTcpSocket* socket;

        //
        QGridLayout* layout;

        //
        QLabel* pseudoLabel;
        //
        QLineEdit* pseudoLineEdit;

        //
        QLabel* portLabel;
        //
        QLineEdit* portLineEdit;

        //
        QLabel* roomNameLabel;
        //
        QLineEdit* roomNameLineEdit;

        //
        QLabel* maxRoundLabel;
        //
        QLineEdit* maxRoundLineEdit;

        //
        QLabel* maxPlayersLabel;
        //
        QLineEdit* maxPlayersLineEdit;

        //
        QLabel* timeByRoundLabel;
        //
        QLineEdit* timeByRoundLineEdit;

        //
        QLabel* timeAfterFirstGoodAnswerLabel;
        //
        QLineEdit* timeAfterFirstGoodAnswerLineEdit;


        //
        QWidget* createOrCancel;
            //
            QHBoxLayout* layoutConnectOrCancel;
                //
                QPushButton* cancelButton;
                //
                QPushButton* createButton;

    // Constructor
    public :
        CreateRoomWindow(QTcpSocket* socket, Server* server);

    // Qt slots
    public slots :
        //
        void createRoom();

        //
        void connectOk();
        //
        void connectError();
};



#endif // CREATEROOMWINDOW_H
