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
// (It's executed on its own thread and will delete himself when its task is over)
class UpdateChecker : public QObject
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
		UpdateChecker(QString currentVersion);

	// Destructor
	public :
		~UpdateChecker();

	// Qt slots
	public slots :
		// Ask to github the information about the last release of the program
		void askGithub();

		// Read the reply from github and send it to the GUI thread if there is an update
		void readReply();

	// Signals
	signals :
		void updateToDownload(QJsonDocument jsonReply);
};



#endif // CHECKFORUPDATE_H
