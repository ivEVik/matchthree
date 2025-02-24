#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H


#include <game/piece.h>

#include <map>
#include <QWidget>
#include <string>


#define PIECE_FILE_EXTENSION "webp"
#define ERROR_PIECE_FILE_NAME std::string("piece-error.") + PIECE_FILE_EXTENSION
#define PIECE_RESOURCE_PATH std::string(":/art/pieces")


class PieceWidget : public QWidget
{
public:
	PieceWidget(QWidget* parent = nullptr);

	std::string getImageFilePath(const Piece& piece);

private:
	static const std::map<const PieceType, const std::string> pieceNameSuffixesByType;
	static const std::map<const PieceValue, const std::string> pieceNameSuffixesByValue;
};


#endif // PIECEWIDGET_H
