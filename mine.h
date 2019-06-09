#pragma once

#include <QLabel>
#include <QPixmap>

#include "minestatus.h"

class QPropertyAnimation;

class Mine : public QFrame {
	Q_OBJECT
public:
	explicit Mine(int x, int y, QWidget* parent = nullptr);

	void setDowned(bool down);

	void setBomb(bool _is_bomb);

	bool isDowned() const;

	bool isBomb() const;

	bool isFlag() const;

	bool isNumber() const;

	int getX() const;

	int getY() const;

	MineStatus getStatus() const;

	void setStatus(MineStatus _status);

	void setNearMineCount(MineStatus count) {
		near_mine_count = count;
	}

	MineStatus getNearMineCount() const;

	void reset();

	void setDebugMode(bool enable);

	QPropertyAnimation* fadeOut(int duration = 150, bool is_start = true);

signals:
	void dug(int x, int y);
	void setFlag(bool is_set_flag);

private:
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent*) override;

private:
	bool downed;
	bool is_bomb;
	bool enable_debug_mode;
	MineStatus near_mine_count;
	MineStatus status;
	int x;
	int y;	
};

inline MineStatus Mine::getStatus() const {
	return status;
}

inline MineStatus Mine::getNearMineCount() const {
	return static_cast<MineStatus>(near_mine_count);
}

inline int Mine::getX() const {
	return x;
}

inline int Mine::getY() const {
	return y;
}

inline bool Mine::isFlag() const {
	return status == STATUS_FLAG;
}

inline bool Mine::isBomb() const {
	return is_bomb;
}
