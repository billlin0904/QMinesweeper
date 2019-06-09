#pragma once

#include "resourcemanager.h"
#include "difficultylevels.h"

class Settings {
public:
	static Settings& get() {
		static Settings settings;
		return settings;
	}

	IconTheme icon_theme;
	DifficultyLevels level;
	QSize window_size;
	int width_count;
	int height_count;
	int bomb_count;
	int background_music_volume;
	int effect_sound_volume;

	void saveSettings(QSize app_size);
private:
	Settings();	
	void loadSettings();
};
