#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <game/gamestate.h>

#include <QWidget>


class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(Gamestate& newGamestate, QWidget* parent = nullptr);


protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Gamestate& gamestate;
};


#endif // MAINWINDOW_H
