#include "gamestate.h"

#include <cmath>


Gamestate::Gamestate()
{
	score = 0;
	rng = std::mt19937(std::random_device()());
	board = std::array<std::array<Piece, BOARD_HEIGHT>, BOARD_WIDTH>();

	auto isValid = [this](const int x, const int y, const PieceType type) -> bool
	{
		if(x + 1 >= MINIMUM_PIECES_FOR_MATCH)
		{
			int matchCount = 1;
			for(int t = x - 1; t >= 0; --t)
			{
				if(this->board[t][y].type != type)
					break;

				if(++matchCount >= MINIMUM_PIECES_FOR_MATCH)
					return false;
			}
		}

		if(y + 1 < MINIMUM_PIECES_FOR_MATCH)
			return true;

		int matchCount = 1;
		for(int t = y - 1; t >= 0; --t)
		{
			if(this->board[x][t].type != type)
				break;

			if(++matchCount >= MINIMUM_PIECES_FOR_MATCH)
				return false;
		}

		return true;
	};

	do
	{
		for(size_t y = 0; y < BOARD_HEIGHT; ++y)
		{
			for(size_t x = 0; x < BOARD_WIDTH; ++x)
			{
				PieceType type;

				do
					type = Piece::randomType(rng);
				while(!isValid(x, y, type));

				board[x][y] = Piece(type, Piece::randomValue(rng));
			}
		}
	}
	while(!hasValidMoves());
}

bool Gamestate::hasValidMoves() const
{
	for(size_t x = 0; x < BOARD_WIDTH; ++x)
	{
		for(size_t y = 0; y < BOARD_HEIGHT; ++y)
		{
			if(validMove(x, y, x - 1, y)
			   || validMove(x, y, x + 1, y)
			   || validMove(x, y, x, y - 1)
			   || validMove(x, y, x, y + 1))
			{
				return true;
			}
		}
	}

	return false;
}

bool Gamestate::willMatch(const int x, const int y, const PieceType type, const BoardDirection noCheckDirection) const
{
	if(x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
		return false;

	int matchCount = 1;

	auto hasMatch = [this, x, y, type, &matchCount](const bool checkX, const int increment) -> bool
	{
		for(int t = (checkX ? x : y) + increment;
			increment > 0
				? t < (checkX
					   ? BOARD_WIDTH
					   : BOARD_HEIGHT)
				: t >= 0;
			t += increment)
		{
			if((checkX ? board[t][y] : board[x][t]).type != type)
				break;
			if(++matchCount >= MINIMUM_PIECES_FOR_MATCH)
				return true;
		}
		return false;
	};

	if(noCheckDirection != BoardDirection::X_NEGATIVE && x + matchCount - MINIMUM_PIECES_FOR_MATCH >= 0 && hasMatch(true, -1))
		return true;
	if(noCheckDirection != BoardDirection::X_POSITIVE && x - matchCount + MINIMUM_PIECES_FOR_MATCH < BOARD_WIDTH && hasMatch(true, 1))
		return true;

	matchCount = 1;

	if(noCheckDirection != BoardDirection::Y_NEGATIVE &&  y + matchCount - MINIMUM_PIECES_FOR_MATCH >= 0 && hasMatch(false, -1))
		return true;
	if(noCheckDirection != BoardDirection::Y_POSITIVE && y - matchCount + MINIMUM_PIECES_FOR_MATCH < BOARD_HEIGHT && hasMatch(false, 1))
		return true;

	return false;
}

bool Gamestate::validMove(const int originX, const int originY, const int targetX, const int targetY) const
{
	if(originX < 0 || originX >= BOARD_WIDTH || originY < 0 || originY >= BOARD_HEIGHT)
		return false;

	if(targetX < 0 || targetX >= BOARD_WIDTH || targetY < 0 || targetY >= BOARD_HEIGHT)
		return false;

	BoardDirection noCheckDirection = BoardDirection::NONE;

	const int xDiff = targetX - originX;
	const int yDiff = targetY - originY;

	if(std::abs(xDiff) + std::abs(yDiff) != 1)
		return false;

	if(xDiff > 0)
		noCheckDirection = BoardDirection::X_NEGATIVE;
	else if(xDiff < 0)
		noCheckDirection = BoardDirection::X_POSITIVE;
	else if(yDiff > 0)
		noCheckDirection = BoardDirection::Y_NEGATIVE;
	else
		noCheckDirection = BoardDirection::Y_POSITIVE;

	return willMatch(targetX, targetY, board[originX][originY].type, noCheckDirection) || willMatch(originX, originY, board[targetX][targetY].type, oppositeDir(noCheckDirection));
}

BoardDirection Gamestate::oppositeDir(const BoardDirection direction)
{
	switch(direction)
	{
	case BoardDirection::X_POSITIVE:
		return BoardDirection::X_NEGATIVE;

	case BoardDirection::X_NEGATIVE:
		return BoardDirection::X_POSITIVE;

	case BoardDirection::Y_NEGATIVE:
		return BoardDirection::Y_POSITIVE;

	case BoardDirection::Y_POSITIVE:
		return BoardDirection::Y_NEGATIVE;

	default:
		return BoardDirection::NONE;
	}
}

bool Gamestate::tryMakeMove(const int originX, const int originY, const int targetX, const int targetY)
{
	if(!validMove(originX, originY, targetX, targetY))
		return false;

	const Piece piece = board[originX][originY];
	board[originX][originY] = board[targetX][targetY];
	board[targetX][targetY] = piece;

	return true;
}

bool Gamestate::process()
{
	std::vector<std::array<size_t, 2>> piecesToDelete = std::vector<std::array<size_t, 2>>();

	bool matchesFound = false;
	auto prevPieces = std::array<std::array<size_t, 2>, MINIMUM_PIECES_FOR_MATCH>();
	size_t prevPieceIndex = 0;
	PieceType prevType = PieceType::NONE;
	int matchCount = 0;

	const auto findMatch = [this, &matchesFound, &piecesToDelete, &prevPieces, &prevPieceIndex, &prevType, &matchCount](const size_t x, const size_t y) -> void
	{
		if(prevType != board[x][y].type)
			matchCount = 0;

		++matchCount;
		++prevPieceIndex;

		if(prevPieceIndex >= MINIMUM_PIECES_FOR_MATCH)
			prevPieceIndex = 0;

		prevType = board[x][y].type;
		prevPieces[prevPieceIndex] = { x, y };

		if(matchCount == MINIMUM_PIECES_FOR_MATCH)
		{
			matchesFound = true;
			for(size_t t = 0; t < MINIMUM_PIECES_FOR_MATCH; ++t)
				piecesToDelete.push_back(prevPieces[t]);
		}
		else if(matchCount > MINIMUM_PIECES_FOR_MATCH)
			piecesToDelete.push_back(prevPieces[prevPieceIndex]);
	};

	for(size_t x = 0; x < BOARD_WIDTH; ++x)
	{
		for(size_t y = 0; y < BOARD_HEIGHT; ++y)
			findMatch(x, y);
	}

	prevType = PieceType::NONE;
	matchCount = 0;

	for(size_t y = 0; y < BOARD_HEIGHT; ++y)
	{
		for(size_t x = 0; x < BOARD_WIDTH; ++x)
			findMatch(x, y);
	}

	for(const std::array<size_t, 2>& coords : piecesToDelete)
	{
		score += static_cast<int>(board[coords[0]][coords[1]].pointValue);
		board[coords[0]][coords[1]] = Piece();
	}


	for(size_t x = 0; x < BOARD_WIDTH; ++x)
	{
		int moveDown = 0;
		for(size_t y = 0; y < BOARD_HEIGHT; ++y)
		{
			if(board[x][y].type == PieceType::NONE)
			{
				++moveDown;
				continue;
			}

			if(!moveDown)
				continue;

			board[x][y - moveDown] = board[x][y];
			//board[x][y] = Piece();
		}

		for(size_t y = BOARD_HEIGHT - moveDown; y < BOARD_HEIGHT; ++y)
			board[x][y] = Piece(rng);
	}


	return matchesFound;
}

int Gamestate::getScore() const { return score; }

const Piece* Gamestate::getPiece(const size_t x, const size_t y) const
{
	if(x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
		return nullptr;
	return &board[x][y];
}
