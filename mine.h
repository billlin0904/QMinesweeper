#pragma once

#include <QFrame>
#include <QPixmap>

enum MineStatus {
	STATUS_BANK = 0,
	STATUS_NUM1,
	STATUS_NUM2,
	STATUS_NUM3,
	STATUS_NUM4,
	STATUS_NUM5,
	STATUS_NUM6,
	STATUS_NUM7,
	STATUS_NUM8,

	STATUS_INIT,
	STATUS_FLAG,
	STATUS_MINE,
};

class Mine : public QFrame {
	Q_OBJECT
public:
	explicit Mine(int x, int y, QWidget* parent = nullptr);

	void setDowned(bool down);

	void setMine(bool _is_mine);

	bool isDowned() const;

	bool isMine() const;

	bool isFlag() const;

	int getX() const;

	int getY() const;

	MineStatus getStatus() const;

	void setStatus(MineStatus _status);

	void setNearMineCount(MineStatus count) {
		near_mine_count = count;
	}

	MineStatus getNearMineCount() const;

signals:
	void dug(int x, int y);

private:
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;

private:
	bool downed;
	bool is_mine;
	MineStatus near_mine_count;
	int x;
	int y;
	MineStatus status;
	QPixmap mine_img;
	QPixmap flag_img;
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