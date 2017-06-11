#ifndef DRAWINGTOOLTYPE_H
#define DRAWINGTOOLTYPE_H



#include <QDataStream>



// All the type of tool to draw on the canvas
enum DrawingToolType : quint16  {PEN,
                                 BUCKET,
                                 ERASER,
                                };

//
Q_DECLARE_METATYPE(DrawingToolType)

//
QDataStream& operator<<(QDataStream& dataStream, DrawingToolType drawingToolType);
//
QDataStream& operator>>(QDataStream& dataStream, DrawingToolType& drawingToolType);



#endif // DRAWINGTOOLTYPE_H
