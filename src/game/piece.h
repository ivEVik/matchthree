#ifndef PIECE_H
#define PIECE_H


#include <random>


enum class PieceType : int
{
	NONE = 0,
	AVOCADO = 1,
	STRAWBERRY = 2,
	LEMON = 3,

	RANDOM_MIN = AVOCADO, // Наименьший по значению тип фишки, который можно получить при случайной генерации.
	RANDOM_MAX = LEMON // Наибольший по значению тип фишки, который можно получить при случайной генерации.
};

// Количество очков за нахождение фишки в комбинации.
enum class PieceValue : int
{
	NONE = 0,
	STANDARD = 1,
	DOUBLE = STANDARD * 2,

	DOUBLE_CHANCE_ONE_IN = 3 // DOUBLE будет появляться один в DOUBLE_CHANCE_ONE_IN раз.
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
