#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <game/gamestate.h>
#include <ui/piecewidget.h>

#include <array>
#include <QWidget>


class MainWindow final : public QWidget
{
	Q_OBJECT

public:
	static constexpr int BOARD_WIDTH = PieceWidget::BOARD_WIDTH;
	static constexpr int BOARD_HEIGHT = PieceWidget::BOARD_HEIGHT;
	static constexpr int MINIMUM_WIDTH = BOARD_WIDTH;
	static constexpr int MINIMUM_HEIGHT = BOARD_HEIGHT;

	MainWindow(Gamestate& newGamestate, QWidget* parent = nullptr);

protected:
	virtual void paintEvent(QPaintEvent* event) override;

private:
	Gamestate& gamestate;
	QWidget* gameBoard;
	PieceWidget* activePiece;

	bool allowBoardInteraction;
	int busyFor;

	std::array<std::array<PieceWidget*, Gamestate::BOARD_HEIGHT>, Gamestate::BOARD_WIDTH> pieceWidgets;

	void movePiece(PieceWidget* piece, const size_t x, const size_t y);
	void movePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);

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


#endif // MAINWINDOW_H
