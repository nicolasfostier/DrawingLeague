#ifndef DATABLOCKTYPE_H
#define DATABLOCKTYPE_H



#include <QDataStream>



//
enum class DataBlockType : quint16 {NOTYPE,
                                    READY_TO_RECEIVE,
                                    PSEUDO_OK,
                                    PSEUDO_ALREADY_USED,
                                    PLAYER_ENTERING,
                                    PLAYER_ONLINE,
                                    PLAYER_LEAVING,
                                    ROOM,
                                    GAME_STARTING,
                                    ROUND_STARTING,
                                    ROUND_ENDING,
                                    SKIP_WORD,
                                    GAME_ENDING,
                                    CHAT,
                                    ANSWER,
                                    ANSWER_FOUND,
                                    DRAWING_TOOL_TYPE,
                                    DRAWING_TOOL_COLOR,
                                    DRAWING_TOOL_WIDTH,
                                    CANVAS_RESET,
                                    CANVAS_MOUSE_PRESS_EVENT,
                                    CANVAS_MOUSE_MOVE_EVENT,
                                    CANVAS_MOUSE_RELEASE_EVENT,
                                    SERVER_MSG_READY_NEEDED,
                                    };

//
Q_DECLARE_METATYPE(DataBlockType)
//
QDataStream& operator<<(QDataStream& dataStream, DataBlockType dataBlockType);
//
QDataStream& operator>>(QDataStream& dataStream, DataBlockType& dataBlockType);



#endif // DATABLOCKTYPE_H
