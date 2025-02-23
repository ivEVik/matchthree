#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <game/gamestate.h>

#include <QMainWindow>


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(Gamestate& gamestate, QWidget *parent = nullptr);
	~MainWindow();

private:
	Gamestate& gamestate;
};


#endif // MAINWINDOW_H
