#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QThread>

// Check if there is an update available on github
#include "include/checkforupdate.h"
// Main window of the program
#include "include/windows/mainwindow.h"



int main(int argc, char *argv[])
{
    // Creation and configuration of the Qt application
    QApplication app(argc, argv);
    app.setApplicationName("Drawing League");
    app.setOrganizationName("Nicolas Fostier");
    app.setOrganizationDomain("nicolasfostier.free.fr");
    app.setApplicationVersion("0.0.1");
    app.setWindowIcon(QIcon(":/images/app.ico"));

    // Force the app to use the same language as the system
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("drawingleague_") + locale, ":/lang");
    app.installTranslator(&translator);

    // Check if there is an update available on github
    // (It's executed on its own thread)
    QThread* threadCFU = new QThread();
    QObject::connect(threadCFU, SIGNAL(finished()), threadCFU, SLOT(deleteLater()));
    CheckForUpdate* cfu = new CheckForUpdate(app.applicationVersion());
    cfu->moveToThread(threadCFU);
    QObject::connect(threadCFU, SIGNAL(started()), cfu, SLOT(askGithub()));
    threadCFU->start();

    // Create the main window of the program and open it
    MainWindow mainWindow;
    mainWindow.show();

    // Execute the Qt application : enter the event loop
    return app.exec();
}
