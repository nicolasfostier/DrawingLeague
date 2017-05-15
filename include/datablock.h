#ifndef DATABLOCK_H
#define DATABLOCK_H



#include <QDataStream>



//
enum class DataBlockType : quint16 {NOTYPE,
                                    NEW_PLAYER,
                                    NEW_MSG_CHAT,
                                    NEW_ANSWER,
                                    ROOM_INFO};

//
Q_DECLARE_METATYPE(DataBlockType)

//
QDataStream& operator<<(QDataStream& dataStream, DataBlockType dataBlockType);
//
QDataStream& operator>>(QDataStream& dataStream, DataBlockType& dataBlockType);



#endif // DATABLOCK_H
