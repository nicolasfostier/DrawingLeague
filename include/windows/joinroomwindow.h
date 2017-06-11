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
#include "include/datablock/datablockreader.h"
//
#include "include/datablock/datablockwriter.h"




class JoinRoomWindow : public QDialog
{
    Q_OBJECT

    // Variables
    private :
        //
        QTcpSocket* socket;

        //
        DataBlockReader* dataBlockReader;
        //
        DataBlockWriter* dataBlockWriter;

        //
        QGridLayout* layout;

            //
            QFont labelFont;

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
        JoinRoomWindow();

    // Qt slots
    public slots:
        //
        void connectToTheServeur();

        //
        void connectOk();
        //
        void connectError();

        //
        void sendPseudo();

        //
        void connectAndPseudoOk();
        //
        void pseudoAlreadyUsed();

    // Signals
    signals :
        void roomJoined(QTcpSocket* socket, DataBlockReader* dataBlockReader, DataBlockWriter* dataBlockWriter, QString pseudo);
};



#endif // JOINROOMWINDOW_H
