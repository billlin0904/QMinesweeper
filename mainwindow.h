#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	enum DifficultyLevels {
		LEVE_BEGINER,
		LEVE_INTERMEDIATE,
		LEVE_EXPORT
	};

	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	void createGame();

	void setEnmoji(int i);

	void loadSettings();

	void saveSettings();

	bool debug_mode;
	DifficultyLevels level;
	QTimer timer;
	QTime base_time;
	QIcon enmoji0;
	QIcon enmoji1;
	QIcon enmoji2;
	Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
