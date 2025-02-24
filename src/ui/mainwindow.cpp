#include "mainwindow.h"

#include <QGraphicsView>
//#include <QGraphicsAnchorLayout>


MainWindow::MainWindow(Gamestate& newGamestate, QWidget* parent) : QWidget(parent), gamestate(newGamestate)
{

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
