#ifndef LOGHANDLER_H
#define LOGHANDLER_H



#include <QtMessageHandler>
#include <QTextStream>
#include <QDateTime>



static const QtMessageHandler defaultMessageHandler = qInstallMessageHandler(0);
extern QTextStream logStream;

// Handle the log of the game
void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);



#endif // LOGHANDLER_H
