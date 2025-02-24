#ifndef PIECE_H
#define PIECE_H


enum class PieceType {
	NONE = 0,
	AVOCADO = 1,
	STRAWBERRY = 2,
	LEMON = 3
};

enum class PieceValue {
	NONE = 0,
	STANDARD = 1,
	DOUBLE = STANDARD * 2
};


struct Piece final
{
public:
	PieceType type;
	PieceValue pointValue;
};


#endif // PIECE_H
