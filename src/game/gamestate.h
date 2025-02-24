#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <game/piece.h>

#include <array>
#include <random>


enum class BoardDirection : int
{
	NONE = 0,
	X_NEGATIVE = 1,
	X_POSITIVE = 2,
	Y_NEGATIVE = 3,
	Y_POSITIVE = 4
};


class Gamestate final
{
public:
	static constexpr int BOARD_WIDTH = 4;
	static constexpr int BOARD_HEIGHT = 5;
	static constexpr int MINIMUM_PIECES_FOR_MATCH = 3;

	Gamestate();

	bool hasValidMoves() const;
	bool willMatch(const int x, const int y, const PieceType type, const BoardDirection noCheckDirection = BoardDirection::NONE) const;
	bool validMove(const int originX, const int originY, const int targetX, const int targetY) const;

	static BoardDirection oppositeDir(const BoardDirection direction);

private:
	std::mt19937 rng;

	std::array<std::array<Piece, BOARD_HEIGHT>, BOARD_WIDTH> board;
};


#endif // GAMESTATE_H
