#include "mainwindow.h"

#include <QPainter>
//#include <QGraphicsAnchorLayout>
#include <QPushButton>
#include <QTimer>


MainWindow::MainWindow(Gamestate& newGamestate, QWidget* parent) : QWidget(parent), gamestate(newGamestate)
{
	activePiece = nullptr;
	pieceWidgets = std::array<std::array<PieceWidget*, Gamestate::BOARD_HEIGHT>, Gamestate::BOARD_WIDTH>();
	allowBoardInteraction = true;
	busyFor = 0;

	setMinimumWidth(MINIMUM_WIDTH);
	setMinimumHeight(MINIMUM_HEIGHT);

	resize(MINIMUM_WIDTH, MINIMUM_HEIGHT);

	gameBoard = new QWidget(this);
	gameBoard->setMinimumWidth(BOARD_WIDTH);
	gameBoard->setMaximumWidth(BOARD_WIDTH);
	gameBoard->setMinimumHeight(BOARD_HEIGHT);
	gameBoard->setMaximumHeight(BOARD_HEIGHT);
	gameBoard->setGeometry(0, 0, BOARD_WIDTH, BOARD_HEIGHT);

	connect(&gamestate, SIGNAL(pieceCreated(const size_t, const size_t)), this, SLOT(slotPieceCreated(const size_t, const size_t)));
	connect(&gamestate, SIGNAL(piecesCreated()), this, SLOT(slotWaitForAnimations()));
	connect(&gamestate, SIGNAL(pieceDeleted(const size_t, const size_t)), this, SLOT(slotPieceDeleted(const size_t, const size_t)));
	connect(&gamestate, SIGNAL(pieceMatched(const size_t, const size_t)), this, SLOT(slotPieceMatched(const size_t, const size_t)));
	connect(&gamestate, SIGNAL(piecesMoved(const size_t, const size_t, const size_t, const size_t)), this, SLOT(slotMovePieces(const size_t, const size_t, const size_t, const size_t)));
	connect(&gamestate, SIGNAL(pieceMoved(const size_t, const size_t, const size_t, const size_t)), this, SLOT(slotMovePiece(const size_t, const size_t, const size_t, const size_t)));
}

void MainWindow::paintEvent(QPaintEvent* event)
{
	QRect rectToPaint = rect();

	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QImage(":/art/background.webp")));
	painter.drawRect(rectToPaint);

	QImage bgHeader = QImage(":/art/background-header.webp");
	rectToPaint.setHeight(bgHeader.height());
	painter.setBrush(QBrush(bgHeader));
	painter.drawRect(rectToPaint);

	QWidget::paintEvent(event);
}

void MainWindow::movePiece(PieceWidget* piece, const size_t x, const size_t y)
{
	if(!piece)
		return;

	if(x >= pieceWidgets.max_size() || y >= pieceWidgets[x].max_size())
		return;

	pieceWidgets[x][y] = piece;
	piece->moveOnBoard(busyFor, x, y);
}

void MainWindow::movePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY)
{
	if(originX >= pieceWidgets.max_size() || originY >= pieceWidgets[originX].max_size() || targetX >= pieceWidgets.max_size() || targetY >= pieceWidgets[originX].max_size())
		return;

	PieceWidget* piece = pieceWidgets[originX][originY];
	movePiece(pieceWidgets[targetX][targetY], originX, originY);
	movePiece(piece, targetX, targetY);
}

void MainWindow::slotPieceCreated(const size_t x, const size_t y)
{
	if(x >= pieceWidgets.max_size() || y >= pieceWidgets[x].max_size())
		return;

	pieceWidgets[x][y] = new PieceWidget(gamestate.getPiece(x, y), gameBoard);
	pieceWidgets[x][y]->instantMoveOnBoard(x, y + Gamestate::BOARD_HEIGHT);
	movePiece(pieceWidgets[x][y], x, y);
	pieceWidgets[x][y]->show();
	connect(pieceWidgets[x][y], SIGNAL(pieceClicked(PieceWidget* const )), this, SLOT(slotPieceWidgetClicked(PieceWidget* const )));
}

void MainWindow::slotPieceWidgetClicked(PieceWidget* const piece)
{
	if(!allowBoardInteraction)
	{
		piece->setChecked(false);
		return;
	}

	if(!activePiece)
	{
		activePiece = piece;
		return;
	}

	bool moved = gamestate.tryMakeMove(activePiece->boardX, activePiece->boardY, piece->boardX, piece->boardY);

	activePiece->setChecked(false);
	piece->setChecked(false);
	activePiece = nullptr;

	if(!moved)
		return;

	slotWaitForAnimations();
}

void MainWindow::slotMovePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY)
{
	movePieces(originX, originY, targetX, targetY);
}

void MainWindow::slotMovePiece(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY)
{
	if(originX >= pieceWidgets.max_size() || originY >= pieceWidgets[originX].max_size())
		return;
	movePiece(pieceWidgets[originX][originY], targetX, targetY);
}

void MainWindow::slotPieceDeleted(const size_t x, const size_t y)
{
	if(x >= pieceWidgets.max_size() || y >= pieceWidgets[x].max_size())
		return;

	delete pieceWidgets[x][y];
}

void MainWindow::slotPieceMatched(const size_t x, const size_t y)
{
	if(x >= pieceWidgets.max_size() || y >= pieceWidgets[x].max_size())
		return;

	pieceWidgets[x][y]->match();
}

void MainWindow::slotProcessGamestate()
{
	busyFor = 0;

	if(gamestate.process())
		return;

	allowBoardInteraction = true;
}

void MainWindow::slotWaitForAnimations()
{
	allowBoardInteraction = false;
	QTimer::singleShot(busyFor, this, SLOT(slotProcessGamestate()));
}
