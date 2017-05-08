#include "include/checkforupdate.h"



// Constructor
CheckForUpdate::CheckForUpdate(QString currentVersion)
{
    this->currentVersion = currentVersion;
}



// Destructor
CheckForUpdate::~CheckForUpdate()
{
    delete manager;
    this->thread()->quit();
}



// Qt slots

// Ask to github the information about the last release of the program
void CheckForUpdate::askGithub(){
    this->manager = new QNetworkAccessManager();

    QNetworkRequest request;
    request.setUrl(QString("https://api.github.com/repos/nicolasfostier/DrawingLeague/releases/latest"));

    reply = manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(processReply()));
}

// Read and process the reply from github
void CheckForUpdate::processReply()
{
    // Check if everything went well
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QString lastVersionAvailable = jsonReply.object().value("tag_name").toString();
        if(lastVersionAvailable > currentVersion)
        {
            QMessageBox().about(nullptr, QObject::tr("Update available !"),"<p>" + QObject::tr("A new version of Drawing League is available on github.") + "<br/>" +
                                                                "<b>" + QObject::tr("Your version :") + "</b> " + currentVersion + "<br/>" +
                                                                "<b>" + QObject::tr("Latest version available :") + "</b> " + jsonReply.object().value("tag_name").toString() + "</p>" +
                                                                "<h3 style='text-align: center'><a href='" + jsonReply.object().value("html_url").toString() + "'>" + QObject::tr("Download the latest version") + "</a></h3>");
        }
    }

    // The class have finish
    this->deleteLater();
}