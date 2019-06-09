#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QDebug>
#include "resourcemanager.h"
#include "mine.h"

Mine::Mine(int x, int y, QWidget* parent)
	: QFrame(parent)
	, downed(false)
	, is_bomb(false)
	, enable_debug_mode(false)
	, near_mine_count(STATUS_BANK)
	, status(STATUS_INIT)
	, x(x)
	, y(y) {
	setFrameStyle(QFrame::NoFrame);
	setObjectName("mine");
}

void Mine::reset() {
	setStatus(STATUS_INIT);
	downed = false;
	is_bomb = false;
}

void Mine::setBomb(bool _is_bomb) {
	is_bomb = _is_bomb;
}

bool Mine::isDowned() const {
	return downed;
}

void Mine::setStatus(MineStatus _status) {
	status = _status;
	update();
}

void Mine::setDebugMode(bool enable) {
	enable_debug_mode = enable;
}

bool Mine::isNumber() const {
	return status >= STATUS_NUM1 && status <= STATUS_NUM8;
}

void Mine::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	auto rc = rect();

	if (isNumber()) {
		painter.drawPixmap(rc, ResourceManager::get().getStatusImage(status));
	} else if (status == STATUS_FLAG) {
		painter.drawPixmap(rc, ResourceManager::get().getStatusImage(STATUS_FLAG));
	} else if (status == STATUS_BOMB) {		
		painter.drawPixmap(rc, ResourceManager::get().getStatusImage(STATUS_BOMB));
	} else if (status == STATUS_BANK) {
		painter.drawPixmap(rc, ResourceManager::get().getStatusImage(STATUS_BANK));
	} else if (status == STATUS_INIT) {
		painter.drawPixmap(rc, ResourceManager::get().getStatusImage(STATUS_INIT));
	}

	if (enable_debug_mode && isBomb()) {
		painter.setPen(QPen(QBrush(Qt::red), 1));
		painter.drawRoundRect(0, 0, width() - 1, height() - 1, 2, 2);
	}
}

void Mine::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		emit dug(x, y);
	}
	else if (event->button() == Qt::RightButton) {
		if (status == STATUS_INIT) {
			setStatus(STATUS_FLAG);
			emit setFlag(true);
		}
		else if (status == STATUS_FLAG) {
			setStatus(STATUS_INIT);
			emit setFlag(false);
		}
	}
}

QPropertyAnimation* Mine::fadeOut(int duration, bool is_start) {
	auto eff = new QGraphicsOpacityEffect(this);
	setGraphicsEffect(eff);

	auto opacity = new QPropertyAnimation(eff, "opacity");
	opacity->setDuration(duration);
	opacity->setStartValue(0);
	opacity->setEndValue(1);
	opacity->setEasingCurve(QEasingCurve::InOutExpo);
	if (is_start) {
		opacity->start(QPropertyAnimation::DeleteWhenStopped);
	}
	return opacity;
}

void Mine::setDowned(bool down) {
	if (status == STATUS_FLAG || isDowned()) {
		return;
	}
	downed = down;
	if (is_bomb) {
		setStatus(STATUS_BOMB);
    }
	fadeOut();	
}
