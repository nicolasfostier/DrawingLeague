#include "include/loghandler.h"


QTextStream logStream;

// Handle the log of the game
void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString msgDateTime = "[" + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "] " + msg;

	// Call the default handler
	(*defaultMessageHandler)(type, context, msgDateTime);

	QByteArray localMsg = msgDateTime.toLocal8Bit();
	logStream << localMsg.constData() << "\n";
	logStream.flush();
}
