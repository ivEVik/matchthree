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

// Класс основного окна.
class MainWindow final : public QWidget
{
	Q_OBJECT

public:
	// Константы размера игровой доски и всего окна.
	static constexpr int BOARD_WIDTH = PieceWidget::BOARD_WIDTH;
	static constexpr int BOARD_HEIGHT = PieceWidget::BOARD_HEIGHT;
	static constexpr int MINIMUM_WIDTH = BOARD_WIDTH + 100;
	static constexpr int MINIMUM_HEIGHT = BOARD_HEIGHT + 240;

	MainWindow(Gamestate& newGamestate, QWidget* parent = nullptr);

	// Деструктор не нужен. Все указатели — на дочерние элементы. Qt их удаляет после удаления родительского элемента.
	//~MainWindow();

	// Располагает дочерние элементы согласно установленным якорям.
	void handleAnchors();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	Gamestate& gamestate;

	// Вспомогательный элемент для упрощения расположения игровой доски.
	QWidget* boardHolder;
	std::vector<std::tuple<QLabel*, Anchor>> boardDecals;
	std::vector<std::tuple<QLabel*, Anchor, int, int>> windowDecals;

	// Игровая доска. На ней перемещаются фрукты-овощи.
	QWidget* gameBoard;
	// Не nullptr если выбран элемент для попытки совершения хода.
	PieceWidget* activePiece;
	// Массив фишек. Соответствует board в gamestate.
	// Фишки убранные с доски удаляются с помощью slotPieceDeleted при получении соответствующего сигнала.
	std::array<std::array<PieceWidget*, Gamestate::BOARD_HEIGHT>, Gamestate::BOARD_WIDTH> pieceWidgets;

	// Устанавливается false при выполнении хода и обработке доски, чтобы игрок не передвигал несколько фишек за раз.
	bool allowBoardInteraction;
	// Сколько нужно ждать конца анимаций на доске.
	int busyFor;

	// Перемещаяет фишку в другую клетку на доске.
	void movePiece(PieceWidget* piece, const size_t x, const size_t y);
	// Меняет две фишки местами.
	void movePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);

	// Применяет якорь к дочернему элементу.
	void applyAnchor(QWidget* const widget, const Anchor anchor, int x, int y);

private slots:
	void slotPieceWidgetClicked(PieceWidget* const piece);

	void slotPieceCreated(const size_t x, const size_t y);
	void slotMovePieces(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);
	void slotMovePiece(const size_t originX, const size_t originY, const size_t targetX, const size_t targetY);
	void slotPieceDeleted(const size_t x, const size_t y);
	void slotPieceMatched(const size_t x, const size_t y);

	// Вызывает обработку доски и проверку на наличие комбинаций фишек.
	// Если что-то произошло, ждёт окончания анимаций и снова пытается обработать доску.
	// Если комбинаций нет, возвращает контроль игроку.
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
