#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QTranslator>



//
#include "include/server/server.h"



//
int main(int argc, char* argv[])
{
    // Creation and configuration of the Qt application
    QCoreApplication app(argc, argv);
    app.setApplicationName("Drawing League Dedicated Server");
    app.setOrganizationName("Nicolas Fostier");
    app.setOrganizationDomain("nicolasfostier.free.fr");
    app.setApplicationVersion("1.0.1");

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

    // Create the command line parser
    QCommandLineParser cmdLineParser;

        //
        cmdLineParser.addHelpOption();

        //
        cmdLineParser.addVersionOption();

        // Name of the room
        QCommandLineOption nameCL(  "name",
                                    QCoreApplication::translate("main", "Room's name."),
                                    QCoreApplication::translate("main", "name"),
                                    QString("Drawing League"));
        cmdLineParser.addOption(nameCL);

        // Port of the server
        QCommandLineOption portCL(  "port",
                                    QCoreApplication::translate("main", "Server's port."),
                                    QCoreApplication::translate("main", "port"),
                                    "23232");
        cmdLineParser.addOption(portCL);

        // Maximum number of players
        QCommandLineOption maxPlayersCL("max-players",
                                        QCoreApplication::translate("main", "Maximum number of players."),
                                        QCoreApplication::translate("main", "number of players"),
                                        "10");
        cmdLineParser.addOption(maxPlayersCL);

        // Standard dictionary
        QCommandLineOption standardDictCL(  "std-dict",
                                            QCoreApplication::translate("main", "Standard dictionary."),
                                            QCoreApplication::translate("main", "easy_french|easy_english"),
                                            "easy_french");
        cmdLineParser.addOption(standardDictCL);

        // Custom dictionary
        QCommandLineOption customDictCL("cst-dict",
                                        QCoreApplication::translate("main", "Custom dictionary."),
                                        QCoreApplication::translate("main", "dictionnary's path"),
                                        "");
        cmdLineParser.addOption(customDictCL);

        // Number of rounds
        QCommandLineOption maxRoundsCL( QStringList() << "max-rounds",
                                        QCoreApplication::translate("main", "Maximum number of rounds."),
                                        QCoreApplication::translate("main", "number of round"),
                                        "10");
        cmdLineParser.addOption(maxRoundsCL);

        // Time by rounds
        QCommandLineOption timeByRoundCL(   QStringList() << "time-round",
                                            QCoreApplication::translate("main", "Time by round."),
                                            QCoreApplication::translate("main", "time in second"),
                                            "180");
        cmdLineParser.addOption(timeByRoundCL);

        // Time remaining after the first player found the word
        QCommandLineOption timeAfterFirstGoodAnswerCL(  QStringList() << "time-after-first-good-answer",
                                                        QCoreApplication::translate("main", "Time remaining after the first player found the word."),
                                                        QCoreApplication::translate("main", "time in second"),
                                                        "30");
        cmdLineParser.addOption(timeAfterFirstGoodAnswerCL);

    // Parse and process the command line arguments
    cmdLineParser.process(app);

    // Get the server's settings
    int port = cmdLineParser.value(portCL).toInt();
    Room room(  cmdLineParser.value(nameCL),
                cmdLineParser.value(maxRoundsCL).toInt(),
                cmdLineParser.value(maxPlayersCL).toInt(),
                cmdLineParser.value(timeByRoundCL).toInt(),
                cmdLineParser.value(timeAfterFirstGoodAnswerCL).toInt());
    QString dictionary;
    if(cmdLineParser.value(customDictCL) != ""){
        dictionary = cmdLineParser.value("cst-dict");
    }
    else if(cmdLineParser.value(standardDictCL) == "easy_french"){
        dictionary = ":/dictionaries/easy_french.txt";
    }
    else{
        dictionary = ":/dictionaries/easy_french.txt";
    }

    // Launch the server
    Server* server = new Server(port, room, dictionary);
    QObject::connect(server, SIGNAL(loadDictionaryFailed()), server, SLOT(deleteLater()));
    QObject::connect(server, SIGNAL(destroyed(QObject*)), &app, SLOT(quit()));

    // Execute the Qt application : enter the event loop
    return app.exec();
}
