#include "soundmanager.h"

SoundManager::SoundManager(bool is_loop, QObject* parent)
	: QObject(parent) 
	, playlist(new QMediaPlaylist()) 
	, player(new QMediaPlayer()) {	
	player->setPlaylist(playlist);
	player->setVolume(100);
	playlist->setPlaybackMode(
		is_loop ? QMediaPlaylist::CurrentItemInLoop : QMediaPlaylist::CurrentItemOnce);
}

void SoundManager::addSound(const QUrl& path) {
	playlist->addMedia(path);
}

void SoundManager::play(int index) {
	playlist->setCurrentIndex(index);

	if (player->state() == QMediaPlayer::PlayingState) {
		player->setPosition(0);
	} else if (player->state() == QMediaPlayer::StoppedState) {
		player->play();
	}
}

void SoundManager::setMuted(bool status) {
	player->setMuted(status);
}