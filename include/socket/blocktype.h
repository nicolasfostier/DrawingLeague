#ifndef DATABLOCKTYPE_H
#define DATABLOCKTYPE_H



#include <QDataStream>



// Types of information that can transit between the server and the clients
enum class BlockType : quint16 {NOTYPE = 0,
								READY_TO_RECEIVE = 1,
								ENTER_THE_GAME = 2,
								HAS_ENTERED_THE_GAME = 3,
								GAME_ERROR = 4,
								PLAYER_ENTERING = 5,
								PLAYER_ONLINE = 6,
								PLAYER_LEAVING = 7,
								ROOM = 8,
								GAME_STARTING = 9,
								ROUND_STARTING = 10,
								ROUND_ENDING = 11,
								SKIP_WORD = 12,
								GAME_ENDING = 13,
								CHAT = 14,
								ANSWER = 16,
								ANSWER_FOUND = 17,
								ANSWER_CLOSE = 28,
								HINT = 18,
								READY_NEEDED = 26,
								READY = 27,
								DRAWING_TOOL_TYPE = 19,
								DRAWING_TOOL_COLOR = 20,
								DRAWING_TOOL_WIDTH = 21,
								CANVAS_RESET = 22,
								CANVAS_MOUSE_PRESS_EVENT = 23,
								CANVAS_MOUSE_MOVE_EVENT = 24,
								CANVAS_MOUSE_RELEASE_EVENT = 25,
								};

Q_DECLARE_METATYPE(BlockType)

QDataStream& operator<<(QDataStream& dataStream, BlockType dataBlockType);
QDataStream& operator>>(QDataStream& dataStream, BlockType& dataBlockType);



#endif // DATABLOCKTYPE_H
