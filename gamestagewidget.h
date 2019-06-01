#pragma once

#include <vector>
#include <memory>
#include <array>

#include <QWidget>
#include <QGridLayout>

#include "mine.h"

class GameStageWidget : public QFrame {
    Q_OBJECT
public:
    explicit GameStageWidget(QWidget *parent = nullptr);

    void create(int _M, int _N, int _max_mine);

	int getMaxMine() const {
		return max_mine;
	}

signals:
    void start(int max_mine);

    void mineCountChanged(int count);

    void stop();

private:
    void resizeEvent(QResizeEvent *) override;

    void randomMine();

	std::vector<const Mine*> getNearMine(int x, int y) const;

    void onDug(int x, int y);

    void dug(int x, int y);       

    int N;
    int M;
    int max_mine;
	int remain_mine;
    std::unique_ptr<QGridLayout> layout;
    std::vector<std::vector<std::unique_ptr<Mine>>> mines;
};
