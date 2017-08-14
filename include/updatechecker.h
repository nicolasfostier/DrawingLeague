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
		QString currentVersion;
		QNetworkAccessManager* manager;
		QNetworkReply* reply;

	// Constructor
	public :
		UpdateChecker(QString currentVersion);

	// Destructor
	public :
		~UpdateChecker();

	// Qt slots
	public slots :
		void askGithub();

		void readReply();

	// Signals
	signals :
		void updateToDownload(QJsonDocument jsonReply);
};



#endif // CHECKFORUPDATE_H
