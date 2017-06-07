#ifndef DATABLOCKTYPE_H
#define DATABLOCKTYPE_H



#include <QDataStream>



//
enum class DataBlockType : quint16 {NOTYPE,
                                    ROOM,
                                    PLAYER_ENTERING,
                                    PLAYER_LEAVING,
                                    ROUND_STARTING,
                                    ROUND_ENDING,
                                    CHAT,
                                    ANSWER,
                                    ANSWER_FOUND,
                                    DRAWING_TOOL_TYPE,
                                    DRAWING_TOOL_COLOR,
                                    DRAWING_TOOL_WIDTH,
                                    SKIP_WORD,
                                    CANVAS_RESET,
                                    CANVAS_MOUSE_PRESS_EVENT,
                                    CANVAS_MOUSE_MOVE_EVENT,
                                    CANVAS_MOUSE_RELEASE_EVENT,
                                    SERVER_MSG_TYPE_READY,
                                    SERVER_MSG_READY_NEEDED,
                                    };

//
Q_DECLARE_METATYPE(DataBlockType)
//
QDataStream& operator<<(QDataStream& dataStream, DataBlockType dataBlockType);
//
QDataStream& operator>>(QDataStream& dataStream, DataBlockType& dataBlockType);



#endif // DATABLOCKTYPE_H
