#pragma once

#include <memory>
#include <vector>
#include <QUrl>

enum SoundIndex {
	BACKGROUND_MUSIC_INDEX = 0,
	DUG_INDEX,
	FLAG_INDEX,
	GAME_OVER_INDEX,
	GAME_WIN_INDEX,
};

class SoundManager {
public:
	static SoundManager& get() {
		static SoundManager manager;
		return manager;
	}
    
    ~SoundManager();

    void addSound(const QUrl& path, bool is_loop);

	void play(int index);

    void setMuted(int index, bool status);

    void setVolume(int index, int vol);

private:
	SoundManager();

    class SoundPlayer;
    std::vector<std::unique_ptr<SoundPlayer>> playlist;
};

