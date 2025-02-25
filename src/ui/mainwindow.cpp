#include "mainwindow.h"

#include <QPainter>
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

	//Дочерний объект будет подчищен qt.
	QLabel* decal = new QLabel(this);
	decal->setPixmap(QPixmap(":/art/settings.webp"));
	windowDecals.push_back(std::make_tuple(decal, Anchor::BOTTOM | Anchor::LEFT, 30, -20));

	//Дочерний объект будет подчищен qt.
	decal = new QLabel(this);
	decal->setPixmap(QPixmap(":/art/key.webp"));
	windowDecals.push_back(std::make_tuple(decal, Anchor::BOTTOM | Anchor::RIGHT, -30, -20));

	//Дочерний объект будет подчищен qt.
	decal = new QLabel(this);
	decal->setPixmap(QPixmap(":/art/status.webp"));
	windowDecals.push_back(std::make_tuple(decal, Anchor::TOP | Anchor::CENTRE_H, 0, 0));

	//Дочерний объект будет подчищен qt.
	decal = new QLabel(this);
	decal->setPixmap(QPixmap(":/art/level-bar.webp"));
	windowDecals.push_back(std::make_tuple(decal, Anchor::TOP | Anchor::CENTRE_H, 0, 48));

	//Дочерний объект будет подчищен qt.
	boardHolder = new QWidget(this);
	decal = new QLabel(boardHolder);
	decal->setPixmap(QPixmap(":/art/mascot-0.webp"));
	boardDecals.push_back(std::make_tuple(decal, Anchor::TOP | Anchor::LEFT));

	//Дочерний объект будет подчищен qt.
	decal = new QLabel(boardHolder);
	decal->setPixmap(QPixmap(":/art/mascot-1.webp"));
	boardDecals.push_back(std::make_tuple(decal, Anchor::TOP | Anchor::RIGHT));

	//Дочерний объект будет подчищен qt.
	decal = new QLabel(this);
	decal->setPixmap(QPixmap(":/art/level-panel.webp"));
	windowDecals.push_back(std::make_tuple(decal, Anchor::TOP | Anchor::CENTRE_H, 0, 96));

	//Дочерний объект будет подчищен qt.
	gameBoard = new QWidget(boardHolder);
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

void MainWindow::resizeEvent(QResizeEvent* event)
{
	handleAnchors();

	QWidget::resizeEvent(event);
}

void MainWindow::handleAnchors()
{
	boardHolder->setGeometry((rect().width() - BOARD_WIDTH) / 2, 0, BOARD_WIDTH, rect().height());
	gameBoard->setGeometry(0, (rect().height() - BOARD_HEIGHT / 1.2) / 2, BOARD_WIDTH, BOARD_HEIGHT);

	for(const auto& [decal, anchor] : boardDecals)
		applyAnchor(decal, anchor, 0, (rect().height() - BOARD_HEIGHT / 1.2) / 2 - decal->rect().height() / 1.5);

	for(const auto& [decal, anchor, x, y] : windowDecals)
		applyAnchor(decal, anchor, x, y);
}

void MainWindow::applyAnchor(QWidget* const widget, const Anchor anchor, int x, int y)
{
	if((anchor & Anchor::BOTTOM) != Anchor::NONE)
		y += widget->parentWidget()->rect().height() - widget->rect().height();
	else if((anchor & Anchor::CENTRE_V) != Anchor::NONE)
		y += (widget->parentWidget()->rect().height() - widget->rect().height()) / 2;

	if((anchor & Anchor::RIGHT) != Anchor::NONE)
		x += widget->parentWidget()->rect().width() - widget->rect().width();
	else if((anchor & Anchor::CENTRE_H) != Anchor::NONE)
		x += (widget->parentWidget()->rect().width() - widget->rect().width()) / 2;

	widget->move(x, y);
}
