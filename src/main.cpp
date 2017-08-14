#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QThread>
#include <QMessageBox>
#include <QObject>

#include "include/updatechecker.h"
#include "include/windows/mainwindow.h"
#include "include/loghandler.h"



int main(int argc, char *argv[])
{
	QDir logDir(".");
	logDir.mkdir("log");
	QFile logFile("log/log-" + QDate::currentDate().toString(Qt::ISODate) + ".txt");
	logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	logStream.setDevice(&logFile);

	qInstallMessageHandler(logHandler);

	// Creation and configuration of the Qt application
	QApplication app(argc, argv);
	app.setApplicationName("Drawing League");
	app.setOrganizationName("Nicolas Fostier");
	app.setOrganizationDomain("nicolasfostier.free.fr");
	app.setApplicationVersion("1.1");
	app.setWindowIcon(QIcon(":/images/logo.ico"));

	// Force the app to use the same language as the system
	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;
	translator.load(QString("drawingleague_") + locale, ":/lang");
	app.installTranslator(&translator);

	// Register some meta-type
	qRegisterMetaType<Room>();
	qRegisterMetaType<Message>();
	qRegisterMetaType<Player>();
	qRegisterMetaType<DrawingToolType>();
	qRegisterMetaType<ErrorCode>();

	// Serious things begin...
	try{
		MainWindow mainWindow;

		// Check if there is an update available on github
		// (It's executed on its own thread and will delete himself when its task is over)
		UpdateChecker* updateChecker = new UpdateChecker(app.applicationVersion());
		QObject::connect(updateChecker, SIGNAL(updateToDownload(QJsonDocument)), &mainWindow, SLOT(showNewUpdateAvailable(QJsonDocument)));

		// Execute the Qt application : enter the event loop
		return app.exec();
	}
	// Catch unexpected throw of exception
	catch(std::exception exception){
		logFile.close();

		QMessageBox::critical(0, app.translate("main", "Unexpected error"),
							  app.translate("main", "The application has encountered an unexpected error :") + "\n"
							  + exception.what());
		app.quit();
		exit(1);
	}
}
