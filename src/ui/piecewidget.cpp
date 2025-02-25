#include "piecewidget.h"

#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPropertyAnimation>


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

PieceWidget::PieceWidget(const Piece* const piece, QWidget* parent) : QPushButton(parent)
{
	pieceImage = QImage(getImageFilePath(piece).c_str());
	pieceHoverOverlay = QImage(getImageFilePath(piece, PIECE_FILE_SUFFIX_HOVER).c_str());
	pieceActiveOverlay = QImage(getImageFilePath(piece, PIECE_FILE_SUFFIX_ACTIVE).c_str());

	isHovered = false;
	matched = false;

	setMinimumWidth(WIDTH);
	setMaximumWidth(WIDTH);
	setMinimumHeight(HEIGHT);
	setMaximumHeight(HEIGHT);
	resize(WIDTH, HEIGHT);

	setFlat(true);
	setStyleSheet("QPushButton { background-color: transparent }");

	setCheckable(true);

	connect(this, SIGNAL(clicked()), this, SLOT(slotOnClick()));
}

void PieceWidget::moveOnBoard(int& busyFor, int x, int y)
{
	boardX = x;
	boardY = y;

	x = boardX * WIDTH;
	y = BOARD_HEIGHT - boardY * HEIGHT - HEIGHT;

	int duration = (std::abs(x - pos().x()) + std::abs(y - pos().y())) / SPEED;
	busyFor = busyFor < duration ? duration : busyFor;

	//Дочерний объект будет подчищен qt.
	QPropertyAnimation* anim = new QPropertyAnimation(this, "pos", this);
	anim->setDuration(duration);
	anim->setStartValue(pos());
	anim->setEndValue(QPoint(x, y));

	anim->start();
}

void PieceWidget::instantMoveOnBoard(const size_t x, const size_t y)
{
	boardX = x;
	boardY = y;
	move(boardX * WIDTH, BOARD_HEIGHT - boardY * HEIGHT - HEIGHT);
}

void PieceWidget::match()
{
	//Дочерний объект будет подчищен qt.
	QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(this);
	setGraphicsEffect(opacity);

	//Дочерний объект будет подчищен qt.
	QPropertyAnimation* anim = new QPropertyAnimation(opacity, "opacity", opacity);
	anim->setDuration(350);
	anim->setStartValue(1);
	anim->setEndValue(0);

	connect(anim, SIGNAL(finished()), this, SLOT(slotDelete()));

	anim->start();
}

std::string PieceWidget::getImageFilePath(const Piece* const piece, const std::string extraSuffix)
{
	if(!piece || piece->type == PieceType::NONE || !pieceNameSuffixesByType.count(piece->type) || !pieceNameSuffixesByValue.count(piece->pointValue))
		return PIECE_RESOURCE_PATH + ERROR_PIECE_FILE_NAME;

	return PIECE_RESOURCE_PATH
		+ "piece-"
		+ pieceNameSuffixesByType.at(piece->type)
		+ '-'
		+ pieceNameSuffixesByValue.at(piece->pointValue)
		+ extraSuffix
		+ PIECE_FILE_EXTENSION;
}

void PieceWidget::paintEvent(QPaintEvent* event)
{
	QPushButton::paintEvent(event);
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(pieceImage));
	painter.drawRect(rect());

	if(isChecked())
	{
		painter.setBrush(QBrush(pieceActiveOverlay));
		painter.drawRect(rect());
	}
	else if(isHovered)
	{
		painter.setBrush(QBrush(pieceHoverOverlay));
		painter.drawRect(rect());
	}
}

void PieceWidget::enterEvent(QEvent* event)
{
	isHovered = true;

	QPushButton::enterEvent(event);
}

void PieceWidget::leaveEvent(QEvent* event)
{
	isHovered = false;

	QPushButton::leaveEvent(event);
}

void PieceWidget::slotOnClick()
{
	emit pieceClicked(this);
}

void PieceWidget::slotDelete()
{
	deleteLater();
}
