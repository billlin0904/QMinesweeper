#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

#include "mine.h"

Mine::Mine(int x, int y, QWidget *parent)
    : QWidget(parent)
    , downed(false)
    , is_mine(false)
    , near_mine_count(0)
    , x(x)
    , y(y)
    , status(STATUS_INIT) {
}

void Mine::setMine(bool _is_mine) {
    is_mine = _is_mine;
    if (_is_mine) {
        mine_img = QPixmap(":/Resources/img/Resources/mine.png");
    } else {
        mine_img = QPixmap();
    }
}

bool Mine::isMine() const {
    return is_mine;
}

bool Mine::isDowned() const {
    return downed;
}

void Mine::setStatus(Status _status) {
    status = _status;
    qDebug() << "Status:" << status;
    update();
}

void Mine::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    auto rc = rect();

    if (downed) {
        painter.fillRect(0, 0, width() - 1, height() - 1, Qt::lightGray);

        auto font = QApplication::font();
        font.setPixelSize(rc.height());
        painter.setFont(font);

        if (status >= STATUS_NUM1 && status <= STATUS_NUM8) {
            static QColor color_table [] = {
                QColor(),
                QColor(40,  131, 40),
                QColor(30,  144, 255),
                QColor(0,   0,   255),
                QColor(255, 0,   255),
                QColor(212, 96,  96),
                QColor(255, 165, 0),
                QColor(136, 32,  240),
                QColor(255, 0,   0)
            };
            painter.setPen(color_table[status]);
            painter.drawText(rc, Qt::AlignCenter, QString::number(status), &rc);
        }
    }    

    painter.setPen(QPen(QBrush(Qt::gray), 1));
    painter.drawRoundRect(0, 0, width() - 1, height() - 1, 2, 2);

    if (status == STATUS_MINE) {
        painter.drawPixmap(rc, mine_img);
    }
}

void Mine::mousePressEvent(QMouseEvent *event) {
    if (event->button()==Qt::LeftButton) {
        emit dug(x, y);
    }
}

void Mine::mouseReleaseEvent(QMouseEvent *) {

}

void Mine::setDowned(bool down) {
    downed = down;
    if (is_mine) {
        setStatus(STATUS_MINE);
    }
}
