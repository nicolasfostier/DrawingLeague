#ifndef CHECKFORUPDATE_H
#define CHECKFORUPDATE_H


#include <QTranslator>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QThread>



// Check if there is an update available on github
// (It's executed on its own thread)
class CheckForUpdate : public QObject
{
    Q_OBJECT

    // Variable
    private :
        // Current version of the program
        QString currentVersion;
        // Manager for the connection to github
        QNetworkAccessManager* manager;
        // Reply from github
        QNetworkReply* reply;

    // Constructor
    public :
        CheckForUpdate(QString currentVersion);

    // Destructor
    public :
        ~CheckForUpdate();

    // Qt slots
    public slots :
        // Ask to github the information about the last release of the program
        void askGithub();

        // Read and process the reply from github
        void processReply();
};



#endif // CHECKFORUPDATE_H
