#ifndef PIECE_H
#define PIECE_H


#include <random>


enum class PieceType : int
{
	NONE = 0,
	AVOCADO = 1,
	STRAWBERRY = 2,
	LEMON = 3,

	RANDOM_MIN = AVOCADO,
	RANDOM_MAX = LEMON
};

enum class PieceValue : int
{
	NONE = 0,
	STANDARD = 1,
	DOUBLE = STANDARD * 2,

	DOUBLE_CHANCE_ONE_IN = 3
};


struct Piece final
{
public:
	PieceType type;
	PieceValue pointValue;

	Piece();
	Piece(const PieceType type, const PieceValue pointValue);
	Piece(std::mt19937& rng);

	static PieceType randomType(std::mt19937& rng);
	static PieceValue randomValue(std::mt19937& rng);

private:
	static std::uniform_int_distribution<> pieceTypeRange;
	static std::uniform_int_distribution<> doubleChanceOneIn;
};


#endif // PIECE_H
