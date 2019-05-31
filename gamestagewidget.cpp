#include "rng.h"
#include "gamestagewidget.h"

GameStageWidget::GameStageWidget(QWidget *parent)
    : QWidget(parent)   
    , N(0)
    , M(0) {
}

void GameStageWidget::create(int _M, int _N, int _max_mine) {
    layout.reset();

    layout.reset(new QGridLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    mines.clear();
    mines.resize(_N);

    N = _N;
    M = _M;
    max_mine = _max_mine;

    for (auto x = 0; x < N; ++x) {
        std::vector<std::unique_ptr<Mine>> temp;
        for (auto y = 0; y < M; ++y) {
            auto mine = new Mine(x, y, this);            
            temp.emplace_back(mine);
            QObject::connect(mine, &Mine::dug, this, &GameStageWidget::onDug);
            layout->addWidget(mine, x, y);
        }
        mines[x] = std::move(temp);
    }

    randomMine();

    for (auto x = 0; x < N; ++x) {
        for (auto y = 0; y < M; ++y) {
            auto mine_count = 0;
            for (auto mine : getNearMine(x, y)) {
                if (mine->isMine()) {
                    ++mine_count;
                }
            }
            mines[x][y]->setNearMineCount(mine_count);
        }
    }
}

void GameStageWidget::onDug(int x, int y) {
    dug(x, y);
}

void GameStageWidget::dug(int x, int y) {
    auto &mine = mines[x][y];
    mine->setDowned(true);
    if (mine->getStatus() != Mine::STATUS_INIT) {
        return;
    }
    mine->setStatus(static_cast<Mine::Status>(mine->getNearMineCount()));
    if (mine->getStatus() == Mine::STATUS_BANK) {
        for (auto mine : getNearMine(x, y)) {
            dug(mine->getX(), mine->getY());
        }
    }
}

std::vector<Mine*> GameStageWidget::getNearMine(int x, int y) {
    std::vector<Mine*> near_mines;

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
        near_mines.push_back(mines[x + 1][y -  1].get());
    }

    if (y) {
        near_mines.push_back(mines[x][y - 1].get());
    }

    return near_mines;
}

void GameStageWidget::randomMine() {
    RNG random;

    for (auto i = 0, x = 0, y = 0; i < max_mine; ++i) {
        if (mines[x][y]->isMine()) {
            x = random(0, N - 1);
            y = random(0, M - 1);
        }
        mines[x][y]->setMine(true);
    }
}

void GameStageWidget::resizeEvent(QResizeEvent *)  {
    auto w = width() / M;
    auto h = height() / N;

    if (w > h) {
        setGeometry(x() + (width() - h *  M) / 2, y(), h * M, height());
    } else {
        setGeometry(x(), y() + (height()- w * N) / 2, width(), w * N);
    }
}
