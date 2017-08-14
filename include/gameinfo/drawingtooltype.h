#ifndef DRAWINGTOOLTYPE_H
#define DRAWINGTOOLTYPE_H



#include <QDataStream>



// Types of tool that we can use to draw on the canvas
enum DrawingToolType : quint16  {PEN = 0,
								 BUCKET = 1,
								 ERASER = 2,
};

Q_DECLARE_METATYPE(DrawingToolType)

QDataStream& operator<<(QDataStream& dataStream, DrawingToolType drawingToolType);
QDataStream& operator>>(QDataStream& dataStream, DrawingToolType& drawingToolType);



#endif // DRAWINGTOOLTYPE_H
