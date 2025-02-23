#include "mainwindow.h"


MainWindow::MainWindow(Gamestate& gamestate, QWidget* parent) : QMainWindow(parent), gamestate(gamestate)
{
	setWindowIcon(QIcon(":/art/icon.png"));
}

/*MainWindow::~MainWindow()
{
}*/

