#include <QTime>
#include <QMessageBox>
#include "resourcemanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, debug_mode(false) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	ui->setupUi(this);

	setWindowTitle("QMinesweeper");

	auto action1 = new QAction(tr("Beginer"), this);
	QObject::connect(action1, &QAction::triggered, [this]() {
		ui->stage->create(8, 8, 10);
        ui->stage->setDisabled(false);
		ui->stage->setDebugMode(debug_mode);
		});

	auto action2 = new QAction(tr("Intermediate"), this);
	QObject::connect(action2, &QAction::triggered, [this, action2]() {
		ui->stage->create(16, 16, 40);
        ui->stage->setDisabled(false);
		ui->stage->setDebugMode(debug_mode);		
		});

	auto action3 = new QAction(tr("Export"), this);
	QObject::connect(action3, &QAction::triggered, [this]() {
        ui->stage->create(30, 24, 99);
        ui->stage->setDisabled(false);
		ui->stage->setDebugMode(debug_mode);
		});

	auto level_menu = new QMenu(tr("New Game"));
	level_menu->addAction(action1);
	level_menu->addAction(action2);
	level_menu->addAction(action3);

	auto theme_menu = new QMenu(tr("Theme"));
	auto theme1_action = new QAction(tr("theme 1"), this);
	QObject::connect(theme1_action, &QAction::triggered, [this]() {
		ResourceManager::get().changeTheme(ICON_THEME1);
		ui->stage->fadeOut();
		});
	auto theme2_action = new QAction(tr("theme 2"), this);
	QObject::connect(theme2_action, &QAction::triggered, [this]() {
		ResourceManager::get().changeTheme(ICON_THEME2);
		ui->stage->fadeOut();
		});
	auto theme3_action = new QAction(tr("theme 3"), this);
	QObject::connect(theme3_action, &QAction::triggered, [this]() {
		ResourceManager::get().changeTheme(ICON_THEME3);
		ui->stage->fadeOut();
		});
	auto theme4_action = new QAction(tr("theme 4"), this);
	QObject::connect(theme4_action, &QAction::triggered, [this]() {
		ResourceManager::get().changeTheme(ICON_THEME4);
		ui->stage->fadeOut();
		});
	theme_menu->addAction(theme1_action);
	theme_menu->addAction(theme2_action);
	theme_menu->addAction(theme3_action);
	theme_menu->addAction(theme4_action);

	auto debug_mode_menu = new QMenu(tr("Debug mode"));
	auto debug_mode_action = new QAction(tr("Enable"), this);
	debug_mode_action->setCheckable(true);
	QObject::connect(debug_mode_action, &QAction::triggered, [this, debug_mode_action]() {
		debug_mode = debug_mode_action->isChecked();
		ui->stage->setDebugMode(debug_mode);
		});
	debug_mode_menu->addAction(debug_mode_action);

	auto help_menu = new QMenu(tr("Help"));
	auto about_action = new QAction(tr("About"), this);
	QObject::connect(about_action, &QAction::triggered, [this]() {
		QMessageBox::about(this, "QMinesweeper", 
			"<a href='https://github.com/billlin0904/QMinesweeper'>GitHub</a>");
		});
	help_menu->addAction(about_action);

	ui->menuBar->addAction(level_menu->menuAction());
	ui->menuBar->addAction(theme_menu->menuAction());
	ui->menuBar->addAction(debug_mode_menu->menuAction());
	ui->menuBar->addAction(help_menu->menuAction());	

	enmoji0 = QIcon(":/Resources/Resources/imgs/emoji_0.png");
	enmoji1 = QIcon(":/Resources/Resources/imgs/emoji_1.png");
	enmoji2 = QIcon(":/Resources/Resources/imgs/emoji_2.png");

	ui->emojiButton->setStyleSheet("QPushButton#emojiButton { border: none; }");
	ui->emojiButton->setIconSize(ui->emojiButton->size());
    QObject::connect(ui->emojiButton, &QPushButton::pressed, [this]() {
        ui->stage->restart();
		ui->stage->setDebugMode(debug_mode);
        ui->stage->setDisabled(false);
    });
	setEnmoji(1);

    ui->timeLcdNumber->setDigitCount(10);
	ui->timeLcdNumber->setMode(QLCDNumber::Dec);
	ui->timeLcdNumber->setSegmentStyle(QLCDNumber::Flat);

	base_time = QTime::currentTime();

    ui->timeLcdNumber->display("00:00.000");
	QObject::connect(&timer, &QTimer::timeout, [this]() {
		auto curr_time = QTime::currentTime();
		int t = base_time.msecsTo(curr_time);
		QTime show_time(0, 0, 0, 0);
		show_time = show_time.addMSecs(t);
        ui->timeLcdNumber->display(show_time.toString("mm:ss.zzz"));
		});

	timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(10);

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
		});

	QObject::connect(ui->stage, &GameStageWidget::stop, [this]() {
        ui->timeLcdNumber->display("00:00.000");
		timer.stop();		
		});

	QObject::connect(ui->stage, &GameStageWidget::gameWin, [this]() {
		setEnmoji(2);
		timer.stop();
		ui->stage->setDisabled(true);
		});

	QObject::connect(ui->stage, &GameStageWidget::gameOver, [this]() {
		setEnmoji(0);
        timer.stop();
        ui->stage->setDisabled(true);
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
