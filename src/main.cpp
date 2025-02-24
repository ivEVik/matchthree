#include <ui/mainwindow.h>

#include <QApplication>


int main(int argc, char* argv[])
{
	QApplication program{argc, argv};

	Gamestate gamestate = Gamestate();
	MainWindow window{gamestate};
	window.show();

	return program.exec();
}
