#include "include/server/wordtools.h"



QString diacriticLetters;
QStringList noDiacriticLetters;

QString removeAccents(QString s) {
	if (diacriticLetters.isEmpty()) {
		diacriticLetters = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
		noDiacriticLetters	<<"S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"
							<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"
							<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"
							<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"
							<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"
							<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"
							<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y";
	}

	QString output = "";
	int sLength = s.length();
	for(int i = 0; i < sLength; ++i) {
		QChar c = s[i];
		int dIndex = diacriticLetters.indexOf(c);
		if (dIndex < 0){
			output.append(c);
		}
		else{
			QString replacement = noDiacriticLetters[dIndex];
			output.append(replacement);
		}
	}

	return output;
}


// Use the Levenshtein distance
bool isClose(QString word, QString answer){
	if(word.size() > 3){
		int wordSize = word.size();
		int answerSize = answer.size();
		QVector<QVector<int>> matrix(wordSize + 1, QVector<int>(answerSize + 1));

		matrix[0][0] = 0;
		for(int i = 1; i <= wordSize; ++i){
			matrix[i][0] = i;
		}
		for(int j = 1; j <= answerSize; ++j){
			matrix[0][j] = j;
		}

		for(int i = 1; i <= wordSize; ++i){
			for(int j = 1; j <= answerSize; ++j){
				matrix[i][j] = qMin(qMin(matrix[i-1][j] + 1, matrix[i][j-1] + 1), matrix[i-1][j-1] + (word[i-1] == answer[j-1] ? 0 : 1));
			}
		}

		return (static_cast<float>(matrix[wordSize][answerSize]) / qMax(wordSize, answerSize) < 0.34);
	}
	else{
		return false;
	}
}
