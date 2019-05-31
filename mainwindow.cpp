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
}

MainWindow::~MainWindow()
{
    delete ui;
}
