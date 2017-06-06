#ifndef CREATEROOMWINDOW_H
#define CREATEROOMWINDOW_H



#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QComboBox>
#include <QFileDialog>
#include <QStandardPaths>


//
#include "include/server/server.h"



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
        QString dictionaryPath;


        //
        QGridLayout* layout;

        //
        QLabel* pseudoLabel;
        //
        QLineEdit* pseudoLineEdit;

        //
        QLabel* dictionaryTypeLabel;
        //
        QComboBox* dictionaryTypeComboBox;
        //
        QStackedLayout* dictionaryStackedLayout;
            //
            QComboBox* dictionaryStandardComboBox;
            //
            QPushButton* browseDictionaryButton;

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
        CreateRoomWindow();

    // Qt slots
    public slots :
        //
        void browseDictionary();

        //
        void createRoom();

        //
        void dictionaryError();

        //
        void connection();
        //
        void connectOk();
        //
        void connectError();

    // Signals
   signals :
        void roomCreated(Server* server, QTcpSocket* socket, QString pseudo);
};



#endif // CREATEROOMWINDOW_H
