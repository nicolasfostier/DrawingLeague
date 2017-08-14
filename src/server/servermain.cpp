#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QDir>



#include "include/server/server.h"
#include "include/loghandler.h"



//
int main(int argc, char* argv[])
{
	QDir logDir(".");
	logDir.mkdir("log");
	QFile logFile("log/log-" + QDate::currentDate().toString(Qt::ISODate) + ".txt");
	logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	logStream.setDevice(&logFile);

	qInstallMessageHandler(logHandler);

	// Creation and configuration of the Qt application
	QCoreApplication app(argc, argv);
	app.setApplicationName("Drawing League Dedicated Server");
	app.setOrganizationName("Nicolas Fostier");
	app.setOrganizationDomain("nicolasfostier.free.fr");
	app.setApplicationVersion("1.1");

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

	// Create the command line parser
	QCommandLineParser cmdLineParser;

		cmdLineParser.addHelpOption();

		cmdLineParser.addVersionOption();

		QCommandLineOption configFileCL("config-file",
									QCoreApplication::translate("main", "Configuration file"),
									QCoreApplication::translate("main", "path"),
									"");
		cmdLineParser.addOption(configFileCL);

		QCommandLineOption nameCL(  "name",
									QCoreApplication::translate("main", "Room's name."),
									QCoreApplication::translate("main", "name"),
									"Drawing League");
		cmdLineParser.addOption(nameCL);

		QCommandLineOption portCL(  "port",
									QCoreApplication::translate("main", "Server's port."),
									QCoreApplication::translate("main", "port"),
									"23232");
		cmdLineParser.addOption(portCL);

		QCommandLineOption maxPlayersCL("max-players",
										QCoreApplication::translate("main", "Maximum number of players."),
										QCoreApplication::translate("main", "number of players"),
										"10");
		cmdLineParser.addOption(maxPlayersCL);

		QCommandLineOption standardDictCL(  "std-dict",
											QCoreApplication::translate("main", "Standard dictionary."),
											"easy_french|easy_english",
											"easy_french");
		cmdLineParser.addOption(standardDictCL);

		QCommandLineOption customDictCL("cst-dict",
										QCoreApplication::translate("main", "Custom dictionary."),
										QCoreApplication::translate("main", "dictionnary's path"),
										"");
		cmdLineParser.addOption(customDictCL);

		QCommandLineOption numberOfRoundsCL( QStringList() << "max-rounds",
										QCoreApplication::translate("main", "Maximum number of rounds."),
										QCoreApplication::translate("main", "number of round"),
										"10");
		cmdLineParser.addOption(numberOfRoundsCL);

		QCommandLineOption timeByRoundCL(   QStringList() << "time-round",
											QCoreApplication::translate("main", "Time by round."),
											QCoreApplication::translate("main", "time in second"),
											"180");
		cmdLineParser.addOption(timeByRoundCL);

		QCommandLineOption timeAfterFirstGoodAnswerCL(  QStringList() << "time-after-first-good-answer",
														QCoreApplication::translate("main", "Time remaining after the first player found the word."),
														QCoreApplication::translate("main", "time in second"),
														"30");
		cmdLineParser.addOption(timeAfterFirstGoodAnswerCL);

	// Serious things begin...
	try{
		// Parse and process the command line arguments
		cmdLineParser.process(app);

		// Get the server's settings
		int port;
		Room room;
		QString dictionaryStd;
		QString dictionaryPath;

		// Try first by looking for a config file at the path defined in the command line
		QSettings* settingsIni;
		if(cmdLineParser.isSet(configFileCL)){
			settingsIni = new QSettings(cmdLineParser.value(configFileCL));
		}
		// If there is no path defined in the command line, check at the default path
		else{
			settingsIni = new QSettings("DrawingLeagueDedicatedServer.ini", QSettings::IniFormat);
		}
		// Load the value defined in the config file
		port = settingsIni->value("port", 23232).toInt();
		room.setRoomName(settingsIni->value("name", "Drawing League").toString());
		room.setMaxPlayers(settingsIni->value("max-players", 10).toInt());
		room.setNumberOfRounds(settingsIni->value("max-rounds", 10).toInt());
		room.setTimeByRound(settingsIni->value("time-round", 180).toInt());
		room.setTimeAfterFirstGoodAnswer(settingsIni->value("time-after-first-good-answer", 30).toInt());
		dictionaryPath = settingsIni->value("cst-dict", "").toString();
		if(dictionaryPath.isEmpty()){
			dictionaryStd = settingsIni->value("std-dict", "easy_french").toString();
		}


		settingsIni->sync();
		delete settingsIni;

		if(cmdLineParser.isSet(portCL)){
			port = cmdLineParser.value(portCL).toInt();
		}
		if(cmdLineParser.isSet(nameCL)){
			room.setRoomName(cmdLineParser.value(nameCL));
		}
		if(cmdLineParser.isSet(maxPlayersCL)){
			room.setMaxPlayers(cmdLineParser.value(maxPlayersCL).toInt());
		}
		if(cmdLineParser.isSet(numberOfRoundsCL)){
			room.setNumberOfRounds(cmdLineParser.value(numberOfRoundsCL).toInt());
		}
		if(cmdLineParser.isSet(timeByRoundCL)){
			room.setTimeByRound(cmdLineParser.value(timeByRoundCL).toInt());
		}
		if(cmdLineParser.isSet(timeAfterFirstGoodAnswerCL)){
			room.setTimeAfterFirstGoodAnswer(cmdLineParser.value(timeAfterFirstGoodAnswerCL).toInt());
		}

		if(cmdLineParser.isSet(customDictCL)){
			dictionaryPath = cmdLineParser.value(customDictCL);
		}
		else if(cmdLineParser.isSet(standardDictCL)){
			dictionaryStd = cmdLineParser.value(standardDictCL);
		}

		if(dictionaryPath.isEmpty()){
			if(dictionaryStd == "easy_french"){
				dictionaryPath = ":/dictionaries/easy_french.txt";
			}
			else{
				qCritical() << QString("[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "]").toUtf8().data()
							<< "There is no standard dictionary named like that :"
							<< dictionaryStd;
				throw std::invalid_argument(QString("There is no standard dictionary named like that : " + dictionaryStd).toUtf8());
			}
		}

		// Create and launch the server
		Server server(port, room, dictionaryPath);
		server.thread()->start();

		// Execute the Qt application : enter the event loop
		return app.exec();
	}
	// Catch unexpected throw of exception
	catch(std::exception exception){
		app.quit();
		exit(1);
	}
}
