#include "soundmanager.h"

SoundManager::SoundManager(QObject* parent)
	: QObject(parent) 
	, playlist(new QMediaPlaylist()) 
	, player(new QMediaPlayer()) {	
	player->setPlaylist(playlist);
}

void SoundManager::addSound(const QUrl& path, bool loop, int volume) {
	playlist->addMedia(path);	
	player->setVolume(volume);
	playlist->setPlaybackMode(
		loop ? QMediaPlaylist::CurrentItemInLoop : QMediaPlaylist::CurrentItemOnce);
}

void SoundManager::replaceSound(const QUrl& path) {
	playlist->removeMedia(0);
	playlist->addMedia(path);
}

void SoundManager::play() {
	if (player->state() == QMediaPlayer::PlayingState) {
		player->setPosition(0);
	} else if (player->state() == QMediaPlayer::StoppedState) {
		player->play();
	}
}

void SoundManager::setMuted(bool status) {
	player->setMuted(status);
}