#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H


#include <game/gamestate.h>
#include <game/piece.h>

#include <QPushButton>
#include <string>


#define PIECE_FILE_EXTENSION ".webp"
#define ERROR_PIECE_FILE_NAME std::string("piece-error") + PIECE_FILE_EXTENSION
#define PIECE_RESOURCE_PATH std::string(":/art/pieces/")
#define PIECE_FILE_SUFFIX_HOVER "-hover"
#define PIECE_FILE_SUFFIX_ACTIVE "-active"


// Фишка на доске.
class PieceWidget final : public QPushButton
{
	Q_OBJECT

public:
	// Ширина и высота фишки.
	static constexpr int WIDTH = 96;
	static constexpr int HEIGHT = 96;

	// Ширина и высота доски.
	static constexpr int BOARD_WIDTH = WIDTH * Gamestate::BOARD_WIDTH;
	static constexpr int BOARD_HEIGHT = HEIGHT * Gamestate::BOARD_HEIGHT;

	// Скорость движения фишки.
	static constexpr float SPEED = 0.5;

	// Сколько времени уйдёт на анимацию исчезновения фишки.
	static constexpr int FADE_DURATION = WIDTH / SPEED;

	// Координаты на доске.
	size_t boardX;
	size_t boardY;

	PieceWidget(const Piece* const piece = nullptr, QWidget* parent = nullptr);

	// Перемещают фишку на доске соответственно с и без анимации.
	void moveOnBoard(int& busyFor, int x, int y);
	void instantMoveOnBoard(const size_t x, const size_t y);

	// Начинает анимацию исчезновения фишки. По окончании, фишка удаляется.
	void match();

	// Возвращает путь к изображению переданной фишки.
	static std::string getImageFilePath(const Piece* const piece, const std::string extraSuffix = "");

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void enterEvent(QEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;

private:
	// Используются в getImageFilePath()
	static const std::map<const PieceType, const std::string> pieceNameSuffixesByType;
	static const std::map<const PieceValue, const std::string> pieceNameSuffixesByValue;

	QImage pieceImage;
	QImage pieceHoverOverlay;
	QImage pieceActiveOverlay;

	bool isHovered;

signals:
	void pieceClicked(PieceWidget* const piece);

private slots:
	void slotOnClick();
	void slotDelete();
};


#endif // PIECEWIDGET_H
