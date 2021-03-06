#pragma once

#include <vector>
#include <memory>
#include <array>

#include <QWidget>
#include <QGridLayout>

#include "soundmanager.h"
#include "mine.h"

class GameStageWidget : public QFrame {
	Q_OBJECT
public:
	explicit GameStageWidget(QWidget* parent = nullptr);

	void restart();
	
	void create(int _M, int _N, int _max_mine, bool reset_layout = true);

	int getMaxMine() const {
		return max_bomb;
	}

	void setDebugMode(bool enable);

	void fadeOut();

	int getHeightCount() const {
		return N;
	}

	int getWidthCount() const {		
		return M;
	}

	int getBombCount() const {
		return max_bomb;
	}
signals:
	void start(int max_mine);

	void mineCountChanged(int count);

	void moveChanged(int move);

	void stop();

	void gameOver();

	void gameWin();

private:
	void resizeEvent(QResizeEvent*) override;

	void randomMine(const std::vector<const Mine*> *near_mine = nullptr);

	void calcNearMineCount();

	std::vector<const Mine*> getNearMine(int x, int y) const;

	void onDug(int x, int y);

	void dug(int x, int y, bool play_sound = true);

    void showAll();

	int N;
	int M;
	int max_bomb;
	int remain_mine;
	int move;
	std::unique_ptr<QGridLayout> layout;
	std::vector<std::vector<std::unique_ptr<Mine>>> mines;
};
