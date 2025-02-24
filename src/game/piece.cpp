#include "piece.h"


std::uniform_int_distribution<> Piece::pieceTypeRange = std::uniform_int_distribution<>(static_cast<int>(PieceType::RANDOM_MIN), static_cast<int>(PieceType::RANDOM_MAX));
std::uniform_int_distribution<> Piece::doubleChanceOneIn = std::uniform_int_distribution<>(1, static_cast<int>(PieceValue::DOUBLE_CHANCE_ONE_IN));

Piece::Piece()
{
	type = PieceType::NONE;
	pointValue = PieceValue::NONE;
}

Piece::Piece(const PieceType type, const PieceValue pointValue)
{
	this->type = type;
	this->pointValue = pointValue;
}

Piece::Piece(std::mt19937& rng) : Piece(randomType(rng), randomValue(rng)) { }

PieceValue Piece::randomValue(std::mt19937& rng)
{
	return doubleChanceOneIn(rng) == 1 ? PieceValue::DOUBLE : PieceValue::STANDARD;
}

PieceType Piece::randomType(std::mt19937& rng)
{
	return static_cast<PieceType>(pieceTypeRange(rng));
}
