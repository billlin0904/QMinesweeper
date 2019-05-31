#pragma once

#include <vector>
#include <memory>

#include <QWidget>
#include <QGridLayout>

#include "mine.h"

class GameStageWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameStageWidget(QWidget *parent = nullptr);

    void create(int _M, int _N, int _max_mine);

signals:
    void start(int max_mine);

    void mineCountChanged(int count);

    void stop();

private:
    void resizeEvent(QResizeEvent *) override;

    void randomMine();

    std::vector<Mine*> getNearMine(int x, int y);

    void onDug(int x, int y);

    void dug(int x, int y);       

    int N;
    int M;
    int max_mine;
    std::unique_ptr<QGridLayout> layout;
    std::vector<std::vector<std::unique_ptr<Mine>>> mines;
};
