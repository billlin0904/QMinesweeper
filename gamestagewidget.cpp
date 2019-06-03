#include "rng.h"
#include "gamestagewidget.h"

GameStageWidget::GameStageWidget(QWidget* parent)
	: QFrame(parent)
	, N(0)
	, M(0)
	, max_mine(0)
	, remain_mine(0)
	, move(0) {
	setStyleSheet("#stage { background-color: white; }");	

	sound_effect.addSound(QUrl::fromLocalFile("./Resources/sounds/bg.mp3"), true);
    sound_effect.addSound(QUrl::fromLocalFile("./Resources/sounds/click.wav"), false);
    sound_effect.addSound(QUrl::fromLocalFile("./Resources/sounds/flag.wav"), false);
    sound_effect.addSound(QUrl::fromLocalFile("./Resources/sounds/gameover.mp3"), false);	
}

void GameStageWidget::restart() {
	create(M, N, max_mine);
}

void GameStageWidget::create(int _M, int _N, int _max_mine) {
	emit stop();

	layout.reset(new QGridLayout());

	mines.clear();
	mines.resize(_N);

	N = _N;
	M = _M;
	max_mine = _max_mine;
	remain_mine = max_mine;
	move = 0;

	auto i = 0;

	for (auto x = 0; x < N; ++x) {
		std::vector<std::unique_ptr<Mine>> temp;
		for (auto y = 0; y < M; ++y) {
			auto mine = new Mine(x, y, this);
			temp.emplace_back(mine);
			QObject::connect(mine, &Mine::dug, this, &GameStageWidget::onDug);
			QObject::connect(mine, &Mine::setFlag, [this]() {
				sound_effect.play(2);
				});
			layout->addWidget(mine, x, y);
		}
		mines[x] = std::move(temp);
	}

	layout->setSpacing(0);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);

	setLayout(layout.get());

	randomMine();
	calcNearMineCount();

	sound_effect.setVolume(0, 3);
	sound_effect.play(0);

	emit start(max_mine);
	emit mineCountChanged(remain_mine);
}

void GameStageWidget::calcNearMineCount() {
	for (auto x = 0; x < N; ++x) {
		for (auto y = 0; y < M; ++y) {
			auto mine_count = 0;
			for (auto mine : getNearMine(x, y)) {
				if (mine->isMine()) {
					++mine_count;
				}
			}
			mines[x][y]->setNearMineCount(static_cast<MineStatus>(mine_count));
		}
	}
}

void GameStageWidget::onDug(int x, int y) {
	dug(x, y);
}

void GameStageWidget::dug(int x, int y, bool play_sound) {
	if (play_sound) {
		sound_effect.play(1);
	}

	auto& mine = mines[x][y];

	if (mine->isMine()) {
		mine->setDowned(true);
		sound_effect.play(3);
		sound_effect.setMuted(0, true);
		emit gameOver();
		return;
	}

	auto is_mine = mine->getStatus() == STATUS_MINE;
	mine->setDowned(true);

	if (!is_mine && mine->getStatus() == STATUS_MINE) {
		--remain_mine;
		emit mineCountChanged(remain_mine);
	}

	if (mine->getStatus() != STATUS_INIT) {
		return;
	}

	mine->setStatus(mine->getNearMineCount());
	if (mine->getStatus() == STATUS_BANK) {
		for (auto mine : getNearMine(x, y)) {
			dug(mine->getX(), mine->getY(), false);
		}
	}

	emit moveChanged(++move);	
}

std::vector<const Mine*> GameStageWidget::getNearMine(int x, int y) const {
	std::vector<const Mine*> near_mines;

	if (x && y) {
		near_mines.push_back(mines[x - 1][y - 1].get());
	}

	if (x) {
		near_mines.push_back(mines[x - 1][y].get());
	}

	if (x && y < M - 1) {
		near_mines.push_back(mines[x - 1][y + 1].get());
	}

	if (y < M - 1) {
		near_mines.push_back(mines[x][y + 1].get());
	}

	if (x < N - 1 && y < M - 1) {
		near_mines.push_back(mines[x + 1][y + 1].get());
	}

	if (x < N - 1) {
		near_mines.push_back(mines[x + 1][y].get());
	}

	if (x < N - 1 && y) {
		near_mines.push_back(mines[x + 1][y - 1].get());
	}

	if (y) {
		near_mines.push_back(mines[x][y - 1].get());
	}

	return near_mines;
}

void GameStageWidget::randomMine(const std::vector<const Mine*>* skip_mine) {
	RNG random;

	for (auto i = 0, x = 0, y = 0; i < max_mine; ++i) {
		while (mines[x][y]->isMine()) {
			x = random(0, N - 1);
			y = random(0, M - 1);
		}
		mines[x][y]->setMine(true);
	}
}

void GameStageWidget::resizeEvent(QResizeEvent*) {
	auto w = width() / M;
	auto h = height() / N;

	if (w > h) {
		setGeometry(x() + (width() - h * M) / 2, y(), h * M, height());
	}
	else {
		setGeometry(x(), y() + (height() - w * N) / 2, width(), w * N);
	}
}
