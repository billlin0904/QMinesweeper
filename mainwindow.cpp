#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("QMinesweeper");
    ui->stage->create(9, 9, 10);

    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &MainWindow::customContextMenuRequested, [this](const QPoint &pos) {
        QMenu menu(this);
        QAction action1("8 * 8", this);
        QObject::connect(&action1, &QAction::triggered, [this]() {
            ui->stage->create(8, 8, 10);
        });

        QAction action2("16 * 16", this);
        QObject::connect(&action2, &QAction::triggered, [this]() {
            ui->stage->create(16, 16, 40);
        });

        QAction action3("16 * 30", this);
        QObject::connect(&action3, &QAction::triggered, [this]() {
            ui->stage->create(30, 16, 99);
        });

        menu.addAction(&action1);
        menu.addAction(&action2);
        menu.addAction(&action3);
        menu.exec(mapToGlobal(pos));
    });

    QIcon default_enmoji(":/Resources/img/Resources/emoji_1.png");
    ui->emojiButton->setStyleSheet(" QPushButton#emojiButton { border:none; } ");
    ui->emojiButton->setIconSize(ui->emojiButton->size());
    ui->emojiButton->setIcon(default_enmoji);

    ui->timeLcdNumber->setDigitCount(5);
    ui->timeLcdNumber->setMode(QLCDNumber::Dec);
    ui->timeLcdNumber->setSegmentStyle(QLCDNumber::Flat);

    base_time = QTime::currentTime();

    ui->timeLcdNumber->display("00:00");
    QObject::connect(&timer, &QTimer::timeout, [this]() {
        auto curr_time = QTime::currentTime();
        int t = base_time.msecsTo(curr_time);
        QTime show_time(0,0,0,0);
        show_time = show_time.addMSecs(t);
        ui->timeLcdNumber->display(show_time.toString("mm:ss"));
    });

    timer.setInterval(1000);

    QObject::connect(ui->stage, &GameStageWidget::start, [this](int max_mine) {
        ui->mineCountLcdNumber->display(QString::number(max_mine));
        base_time = QTime::currentTime();
        timer.start();
    });

    QObject::connect(ui->stage, &GameStageWidget::stop, [this]() {
        ui->timeLcdNumber->display("00:00");
        timer.stop();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
