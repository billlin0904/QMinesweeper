#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#ifdef Q_OS_WIN
	QFont font("Segoe UI", 10);
	a.setFont(font);
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
