#include <QSettings>
#include "settings.h"

Settings::Settings() {
	loadSettings();
}

void Settings::loadSettings() {
	const auto settings_path = "./QMinesweeper.ini";
	QSettings settings(settings_path, QSettings::IniFormat);
	settings.setIniCodec("UTF-8");

	icon_theme = static_cast<IconTheme>(settings.value("/App/theme/", IconTheme::ICON_THEME1).toInt());
	ResourceManager::get().changeTheme(icon_theme);
	level = static_cast<DifficultyLevels>(settings.value("/App/difficulty/",
		DifficultyLevels::LEVEL_BEGINER).toInt());

	background_music_volume = settings.value("/App/background_music_volume/", 5).toInt();
	effect_sound_volume = settings.value("/App/effect_sound_volume/", 100).toInt();

	const auto default_width = 698;
	const auto default_height = 776;

	window_size = QSize(settings.value("/App/width/", default_width).toInt(),
		settings.value("/App/height/", default_height).toInt());

	switch (level) {
	case LEVEL_BEGINER:
		width_count = 8;
		height_count = 8;
		bomb_count = 10;
		break;
	case LEVEL_INTERMEDIATE:
		width_count = 16;
		height_count = 16;
		bomb_count = 40;
		break;
	case LEVEL_EXPORT:
		width_count = 30;
		height_count = 24;
		bomb_count = 99;
		break;
	case LEVEL_CUSTOM:
		width_count = settings.value("/App/width_count/", 8).toInt();
		height_count = settings.value("/App/height_count/", 8).toInt();
		bomb_count = settings.value("/App/bomb_count/", 10).toInt();
		break;
	}
}

void Settings::saveSettings(QSize app_size) {
	const auto settings_path = "./QMinesweeper.ini";
	QSettings settings(settings_path, QSettings::IniFormat);
	settings.setIniCodec("UTF-8");

	settings.setValue("/App/theme/", ResourceManager::get().getIconTheme());
	settings.setValue("/App/difficulty/", level);
	settings.setValue("/App/width/", app_size.width());
	settings.setValue("/App/height/", app_size.height());

	settings.setValue("/App/width_count/", width_count);
	settings.setValue("/App/height_count/", height_count);
	settings.setValue("/App/bomb_count/", bomb_count);

	settings.setValue("/App/background_music_volume/", background_music_volume);
	settings.setValue("/App/effect_sound_volume/", effect_sound_volume);
}
