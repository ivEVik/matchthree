#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <game/cell.h>

#include <array>


constexpr int GAME_BOARD_WIDTH = 5;
constexpr int GAME_BOARD_HEIGHT = 5;

class Gamestate
{
public:
	Gamestate();

private:
	std::array<std::array<Cell, GAME_BOARD_HEIGHT>, GAME_BOARD_WIDTH> board;
};


#endif // GAMESTATE_H
