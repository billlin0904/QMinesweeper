#pragma once

#include <QObject>
#include <QMediaPlaylist>
#include <QMediaPlayer>

class SoundManager : public QObject {
	Q_OBJECT
public:
	explicit SoundManager(bool is_loop = false, QObject* parent = nullptr);

	void addSound(const QUrl& path);

public slots:
	void play(int index);

	void setMuted(bool status);

private:
	QMediaPlaylist* playlist;
	QMediaPlayer* player;
};

