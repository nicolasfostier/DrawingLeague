#ifndef DATABLOCK_H
#define DATABLOCK_H



#include <QDataStream>



//
enum class DataBlockType : quint16 {NOTYPE,
                                    QSTRING_PSEUDO,
                                    QSTRING_CHAT,
                                    QSTRING_ANSWER};

//
Q_DECLARE_METATYPE(DataBlockType)

//
QDataStream& operator<<(QDataStream& dataStream, DataBlockType dataBlockType);
//
QDataStream& operator>>(QDataStream& dataStream, DataBlockType& dataBlockType);



#endif // DATABLOCK_H
