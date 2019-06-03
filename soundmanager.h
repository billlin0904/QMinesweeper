#pragma once

#include <memory>
#include <vector>
#include <QUrl>
#include <QObject>

class SoundManager : public QObject {
	Q_OBJECT
public:
    explicit SoundManager(QObject* parent = nullptr);

    ~SoundManager();

    void addSound(const QUrl& path, bool is_loop);

public slots:
	void play(int index);

    void setMuted(int index, bool status);

    void setVolume(int index, int vol);

private:
    class SoundPlayer;
    std::vector<std::unique_ptr<SoundPlayer>> playlist;
};

