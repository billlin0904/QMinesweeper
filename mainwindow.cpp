#include <QTime>
#include <QShortcut>
#include <QSettings>
#include <QStyle>
#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDir>
#include "settings.h"
#include "preferencesdialog.h"
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

	auto cheat_mode_enable = new QShortcut(this);
	cheat_mode_enable->setKey(Qt::CTRL + Qt::Key_G);
	QObject::connect(cheat_mode_enable, reinterpret_cast<void (QShortcut::*)(bool)>(&QShortcut::activated), [this](bool) {
		debug_mode = !debug_mode;
		ui->stage->setDebugMode(debug_mode);
		});

	auto help_menu = new QMenu(tr("Help"));
	auto about_action = new QAction(tr("About"), this);
	QObject::connect(about_action, &QAction::triggered, [this]() {
		QMessageBox::about(this, "QMinesweeper", 
			"<a href='https://github.com/billlin0904/QMinesweeper'>GitHub</a>");
		});
	help_menu->addAction(about_action);

	auto preferences_action = new QAction(tr("Preferences"), this);
	QObject::connect(preferences_action, &QAction::triggered, [this]() {
		PreferencesDialog dialog(this);
		dialog.exec();
		auto& settings = Settings::get();
		if (settings.width_count != ui->stage->getWidthCount()
			|| settings.height_count != ui->stage->getHeightCount()
			|| settings.bomb_count != ui->stage->getBombCount()) {
			ui->stage->create(settings.width_count, settings.height_count, settings.bomb_count);
		}
		});
	ui->menuBar->addAction(preferences_action);

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

	loadSettings();
	createGame();

	setStyleSheet("#centralWidget { background-color: white; }");
}

void MainWindow::loadSettings() {
	setGeometry(
		QStyle::alignedRect(
			Qt::LeftToRight,
			Qt::AlignCenter,
			Settings::get().window_size,
			qApp->desktop()->availableGeometry()
		)
	);
}

void MainWindow::saveSettings() {
	Settings::get().saveSettings(size());
}

void MainWindow::createGame() {
	switch (Settings::get().level) {
	case LEVEL_BEGINER:
		ui->stage->create(8, 8, 10);
		break;
	case LEVEL_INTERMEDIATE:
		ui->stage->create(16, 16, 40);
		break;
	case LEVEL_EXPORT:
		ui->stage->create(30, 24, 99);
		break;
	case LEVEL_CUSTOM:
		ui->stage->create(Settings::get().width_count, Settings::get().height_count, Settings::get().bomb_count);
		break;
	}
	ui->stage->setDisabled(false);
	ui->stage->setDebugMode(debug_mode);
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
	saveSettings();
	delete ui;
}
