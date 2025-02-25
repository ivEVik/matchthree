#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <game/gamestate.h>
#include <ui/piecewidget.h>

#include <array>
#include <QLabel>
#include <QWidget>


enum class Anchor
{
	NONE = 0,
	TOP = 1 << 0,
	BOTTOM = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3,
	CENTRE_H = 1 << 4,
	CENTRE_V = 1 << 5
};

class MainWindow final : public QWidget
{
	Q_OBJECT

public:
	static constexpr int BOARD_WIDTH = PieceWidget::BOARD_WIDTH;
	static constexpr int BOARD_HEIGHT = PieceWidget::BOARD_HEIGHT;
	static constexpr int MINIMUM_WIDTH = BOARD_WIDTH + 100;
	static constexpr int MINIMUM_HEIGHT = BOARD_HEIGHT + 240;

	MainWindow(Gamestate& newGamestate, QWidget* parent = nullptr);
	void handleAnchors();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	Gamestate& gamestate;

	QWidget* boardHolder;
	QWidget* gameBoard;
	std::vector<std::tuple<QLabel*, Anchor>> boardDecals;
	std::vector<std::tuple<QLabel*, Anchor, int, int>> windowDecals;

	PieceWidget* activePiece;
	std::array<std::array<PieceWidget*, Gamestate::BOARD_HEIGHT>, Gamestate::BOARD_WIDTH> pieceWidgets;

	bool allowBoardInteraction;
	int busyFor;

	void movePiece(PieceWidget* piece, const size_t x, const size_t y);
	void movePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);

	void applyAnchor(QWidget* const widget, const Anchor anchor, int x, int y);

private slots:
	void slotPieceWidgetClicked(PieceWidget* const piece);

	void slotPieceCreated(const size_t x, const size_t y);
	void slotMovePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);
	void slotMovePiece(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);
	void slotPieceDeleted(const size_t x, const size_t y);
	void slotPieceMatched(const size_t x, const size_t y);

	void slotProcessGamestate();

	void slotWaitForAnimations();
};

inline Anchor operator|(Anchor a, Anchor b)
{
	return static_cast<Anchor>(static_cast<int>(a) | static_cast<int>(b));
}

inline Anchor operator&(Anchor a, Anchor b)
{
	return static_cast<Anchor>(static_cast<int>(a) & static_cast<int>(b));
}


#endif // MAINWINDOW_H
