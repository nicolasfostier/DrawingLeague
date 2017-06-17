#include "include/gameinfo/drawingtooltype.h"



//
QDataStream& operator<<(QDataStream& dataStream, DrawingToolType drawingToolType) {
    return dataStream << static_cast<quint16>(drawingToolType);
}

//
QDataStream& operator>>(QDataStream& dataStream, DrawingToolType& drawingToolType) {
    quint16 value;
    dataStream >> value;

    if (dataStream.status() == QDataStream::Ok){
        drawingToolType = DrawingToolType(value);
    }
    return dataStream;
}
