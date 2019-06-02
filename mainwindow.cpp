#include <QTime>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {

	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	ui->setupUi(this);

	setWindowTitle("QMinesweeper");

	auto action1 = new QAction(tr("Beginer"), this);
	QObject::connect(action1, &QAction::triggered, [this]() {
		ui->stage->create(8, 8, 10);
		});

	auto action2 = new QAction(tr("Intermediate"), this);
	QObject::connect(action2, &QAction::triggered, [this]() {
		ui->stage->create(16, 16, 40);
		});

	auto action3 = new QAction(tr("Export"), this);
	QObject::connect(action3, &QAction::triggered, [this]() {
		ui->stage->create(30, 24, 99);
		});

	auto level_menu = new QMenu(tr("New Game"));
	level_menu->addAction(action1);
	level_menu->addAction(action2);
	level_menu->addAction(action3);

	auto help_menu = new QMenu(tr("Help"));
	auto about_action = new QAction(tr("About"), this);
	QObject::connect(about_action, &QAction::triggered, [this]() {
		QMessageBox::about(this, "QMinesweeper", "<a href='https://github.com/billlin0904/QMinesweeper'>GitHub</a>");
		});
	help_menu->addAction(about_action);

	ui->menuBar->addAction(level_menu->menuAction());
	ui->menuBar->addAction(help_menu->menuAction());

	enmoji0 = QIcon(":/Resources/Resources/imgs/emoji_0.png");
	enmoji1 = QIcon(":/Resources/Resources/imgs/emoji_1.png");
	enmoji2 = QIcon(":/Resources/Resources/imgs/emoji_2.png");

	ui->emojiButton->setStyleSheet("QPushButton#emojiButton { border: none; }");
	ui->emojiButton->setIconSize(ui->emojiButton->size());
	setEnmoji(1);

	ui->timeLcdNumber->setDigitCount(5);
	ui->timeLcdNumber->setMode(QLCDNumber::Dec);
	ui->timeLcdNumber->setSegmentStyle(QLCDNumber::Flat);

	base_time = QTime::currentTime();

	ui->timeLcdNumber->display("00:00");
	QObject::connect(&timer, &QTimer::timeout, [this]() {
		auto curr_time = QTime::currentTime();
		int t = base_time.msecsTo(curr_time);
		QTime show_time(0, 0, 0, 0);
		show_time = show_time.addMSecs(t);
		ui->timeLcdNumber->display(show_time.toString("mm:ss"));
		});

	timer.setInterval(1000);

	QObject::connect(ui->stage, &GameStageWidget::start, [this](int max_mine) {
		ui->mineCountLcdNumber->display(QString::number(max_mine));
		base_time = QTime::currentTime();
		timer.start();
		setEnmoji(1);
		});

	QObject::connect(ui->stage, &GameStageWidget::moveChanged, [this](int move) {
		ui->moveLcdNumber->display(QString::number(move));
		});

	QObject::connect(ui->stage, &GameStageWidget::mineCountChanged, [this](int mine_count) {
		ui->mineCountLcdNumber->display(QString::number(mine_count));
		if (mine_count <= ui->stage->getMaxMine() / 4) {
			setEnmoji(2);
		}
		else if (mine_count <= ui->stage->getMaxMine() / 3) {
			setEnmoji(1);
		}
		});

	QObject::connect(ui->stage, &GameStageWidget::stop, [this]() {
		ui->timeLcdNumber->display("00:00");
		timer.stop();		
		});

	QObject::connect(ui->stage, &GameStageWidget::gameOver, [this]() {
		setEnmoji(0);
		QMessageBox::information(this, "QMinesweeper", tr("Game Over"), QMessageBox::Yes);		
		ui->stage->restart();
		});

	ui->stage->create(8, 8, 10);

	setStyleSheet("#centralWidget { background-color: white; }");
}

void MainWindow::setEnmoji(int i) {
	switch (i) {
	case 0:
		ui->emojiButton->setIcon(enmoji0);
		break;
	case 1:
		ui->emojiButton->setIcon(enmoji1);
		break;
	case 2:
		ui->emojiButton->setIcon(enmoji2);
		break;
	}
}

MainWindow::~MainWindow() {
	delete ui;
}
