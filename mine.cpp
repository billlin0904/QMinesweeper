#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "mine.h"

Mine::Mine(int x, int y, QWidget* parent)
	: QLabel(parent)
	, downed(false)
	, is_mine(false)
	, near_mine_count(STATUS_BANK)
	, x(x)
	, y(y)
	, status(STATUS_INIT) {
	setFrameStyle(QFrame::NoFrame);
	setObjectName("mine");
}

void Mine::reset() {
	setStatus(STATUS_INIT);
	downed = false;
	is_mine = false;
}

void Mine::setMine(bool _is_mine) {
	is_mine = _is_mine;
	if (_is_mine) {
		mine_img = QPixmap(":/Resources/Resources/imgs/mine.png");
	}
	else {
		mine_img = QPixmap();
	}
}

bool Mine::isMine() const {
	return is_mine;
}

bool Mine::isDowned() const {
	return downed;
}

void Mine::setStatus(MineStatus _status) {
	status = _status;
	//qDebug() << "Status:" << status;
	update();
}

bool Mine::isNumber() const {
	return status >= STATUS_NUM1 && status <= STATUS_NUM8;
}

void Mine::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	auto rc = rect();

	//if (downed) {
		//painter.fillRect(0, 0, width() - 1, height() - 1, Qt::lightGray);
	//}

	if (isNumber()) {
		static QString path_table[] = {
			"",
			":/Resources/Resources/imgs/num1.png",
			":/Resources/Resources/imgs/num2.png",
			":/Resources/Resources/imgs/num3.png",
			":/Resources/Resources/imgs/num4.png",
			":/Resources/Resources/imgs/num5.png",
			":/Resources/Resources/imgs/num6.png",
			":/Resources/Resources/imgs/num7.png",
			":/Resources/Resources/imgs/num8.png",
		};
		painter.drawPixmap(rc,
			QPixmap(path_table[status]));
	} else if (status == STATUS_FLAG) {
		if (flag_img.isNull()) {
			flag_img = QPixmap(":/Resources/Resources/imgs/flag.png");
		}
		painter.drawPixmap(rc, flag_img);
	} else if (status == STATUS_BOMB) {
		painter.drawPixmap(rc, mine_img);
	} else if (status == STATUS_BANK) {
		painter.drawPixmap(rc, QPixmap(":/Resources/Resources/imgs/bank.png"));
	} else if (status == STATUS_INIT) {
		painter.drawPixmap(rc, QPixmap(":/Resources/Resources/imgs/init.png"));
	}

	//painter.setPen(QPen(QBrush(Qt::gray), 1));
	//painter.drawRoundRect(0, 0, width() - 1, height() - 1, 2, 2);	
}

void Mine::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		emit dug(x, y);
	}
	else if (event->button() == Qt::RightButton) {
		if (status == STATUS_INIT) {
			setStatus(STATUS_FLAG);
			emit setFlag();
		}
		else if (status == STATUS_FLAG) {
			setStatus(STATUS_INIT);
		}
	}
}

void Mine::setDowned(bool down) {
	if (status == STATUS_FLAG || isDowned()) {
		return;
	}
	downed = down;
	if (is_mine) {
		setStatus(STATUS_BOMB);
    }
}
