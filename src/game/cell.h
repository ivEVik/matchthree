#ifndef CELL_H
#define CELL_H


enum class CellType {
	NONE = 0,
	ONE = 1,
	TWO = 2,
	THREE = 3
};

enum class CellValue {
	NONE = 0,
	DEFAULT = 1,
	DOUBLE = DEFAULT * 2
};


struct Cell
{
public:
	CellType Type;
	CellValue PointValue;
};


#endif // CELL_H
