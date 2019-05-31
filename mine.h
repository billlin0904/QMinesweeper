#pragma once

#include <QWidget>
#include <QPixmap>

class Mine : public QWidget {
    Q_OBJECT
public:
    enum Status {
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

    explicit Mine(int x, int y, QWidget *parent = nullptr);

    void setDowned(bool down);

    void setMine(bool _is_mine);

    void paintEvent(QPaintEvent *) override;

    bool isDowned() const;

    bool isMine() const;

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    Status getStatus() const {
        return status;
    }

    void setStatus(Status _status);

    void setNearMineCount(int count) {
        near_mine_count = count;
    }

    int getNearMineCount() const {
        return near_mine_count;
    }

signals:
    void dug(int x, int y);

private:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    bool downed;
    bool is_mine;
    int near_mine_count;
    int x;
    int y;
    Status status;
    QPixmap mine_img;
};
