#pragma once

#include <QObject>
#include <QMediaPlaylist>
#include <QMediaPlayer>

class SoundManager : public QObject {
	Q_OBJECT
public:
	explicit SoundManager(QObject* parent = nullptr);

	void addSound(const QUrl& path, bool loop = false, int volume = 100);

public slots:
	void replaceSound(const QUrl& path);

	void play();

	void setMuted(bool status);

private:
	QMediaPlaylist* playlist;
	QMediaPlayer* player;
};

