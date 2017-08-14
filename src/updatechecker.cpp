#include "include/updatechecker.h"



// Constructor
UpdateChecker::UpdateChecker(QString currentVersion)
{
	this->currentVersion = currentVersion;

	// Move the object to another thread and start its execution
	QThread* threadCFU = new QThread();
	QObject::connect(threadCFU, SIGNAL(finished()),
					 threadCFU, SLOT(deleteLater()));
	this->moveToThread(threadCFU);
	QObject::connect(threadCFU, SIGNAL(started()),
					 this, SLOT(askGithub()));
	threadCFU->start();
}



// Destructor
UpdateChecker::~UpdateChecker()
{
	delete manager;

	this->thread()->quit();
}



// Qt slots

void UpdateChecker::askGithub(){
	this->manager = new QNetworkAccessManager();

	QNetworkRequest request;
	request.setUrl(QString("https://api.github.com/repos/nicolasfostier/DrawingLeague/releases/latest"));

	reply = manager->get(request);
	QObject::connect(reply, SIGNAL(finished()),
					 this, SLOT(readReply()));
}

void UpdateChecker::readReply()
{
	if(reply->error() == QNetworkReply::NoError)
	{
		QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
		QString lastVersionAvailable = jsonReply.object().value("tag_name").toString();
		if(lastVersionAvailable > currentVersion)
		{
			emit updateToDownload(jsonReply);
		}
	}

	this->deleteLater();
}
