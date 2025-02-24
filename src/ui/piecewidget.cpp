#include "piecewidget.h"


const std::map<const PieceType, const std::string> PieceWidget::pieceNameSuffixesByType =
{
	{ PieceType::AVOCADO, "avocado" },
	{ PieceType::STRAWBERRY, "strawberry" },
	{ PieceType::LEMON, "lemon" }
};

const std::map<const PieceValue, const std::string> PieceWidget::pieceNameSuffixesByValue =
{
	{ PieceValue::STANDARD, "standard" },
	{ PieceValue::DOUBLE, "double" }
};

PieceWidget::PieceWidget(QWidget* parent) : QWidget(parent) { }

std::string PieceWidget::getImageFilePath(const Piece& piece)
{
	if(!pieceNameSuffixesByType.count(piece.type) || !pieceNameSuffixesByValue.count(piece.pointValue))
		return PIECE_RESOURCE_PATH + ERROR_PIECE_FILE_NAME;
	return PIECE_RESOURCE_PATH + "piece-" + pieceNameSuffixesByType.at(piece.type) + '-' + pieceNameSuffixesByValue.at(piece.pointValue);
}
