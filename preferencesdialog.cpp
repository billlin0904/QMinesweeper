#include "settings.h"
#include "soundmanager.h"
#include "ui_preferencesdialog.h"
#include "preferencesdialog.h"

static int getMaxBomb(int width, int height) {
	return static_cast<int>(double(width * height) * 0.15);
}

PreferencesDialog::PreferencesDialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::PreferencesDialog) {
	ui->setupUi(this);
	ui->widthSpinBox->setValue(Settings::get().width_count);
	ui->heightSpinBox->setValue(Settings::get().height_count);
	ui->bombSpinBox->setValue(Settings::get().bomb_count);
	ui->backgroundMusicVolumeSlider->setValue(Settings::get().background_music_volume);
	ui->effectSoundSlider->setValue(Settings::get().effect_sound_volume);
	ui->bombIcon->setPixmap(ResourceManager::get().getBombIcon());

	int current_theme_index = 0;
	for (auto i = 0; i <= ICON_THEME4; ++i) {		
		ui->themeComboBox->addItem(ResourceManager::get().getThemeIcon(IconTheme(i)), "");
		if (ResourceManager::get().getIconTheme() == i) {
			current_theme_index = i;
		}
	}
	ui->themeComboBox->setCurrentIndex(current_theme_index);
	ui->levelComboBox->setCurrentIndex(Settings::get().level);
	if (Settings::get().level != LEVEL_CUSTOM) {
		ui->widthSpinBox->setDisabled(true);
		ui->heightSpinBox->setDisabled(true);
		ui->bombSpinBox->setDisabled(true);
	}

	QObject::connect(ui->levelComboBox, (void(QComboBox::*)(int)) & QComboBox::activated, [this](int value) {
		Settings::get().level = DifficultyLevels(value);

		if (Settings::get().level != LEVEL_CUSTOM) {
			ui->widthSpinBox->setDisabled(true);
			ui->heightSpinBox->setDisabled(true);
			ui->bombSpinBox->setDisabled(true);
		}
		else {
			ui->widthSpinBox->setDisabled(false);
			ui->heightSpinBox->setDisabled(false);
			ui->bombSpinBox->setDisabled(false);			
		}

		switch (value) {
		case LEVEL_BEGINER:
			ui->widthSpinBox->setValue(8);
			ui->heightSpinBox->setValue(8);
			ui->bombSpinBox->setValue(10);
			break;
		case LEVEL_INTERMEDIATE:
			ui->widthSpinBox->setValue(16);
			ui->heightSpinBox->setValue(16);
			ui->bombSpinBox->setValue(40);			
			break;
		case LEVEL_EXPORT:			
			ui->widthSpinBox->setValue(30);
			ui->heightSpinBox->setValue(24);
			ui->bombSpinBox->setValue(99);
			break;
		case LEVEL_CUSTOM:
			break;
		}
		});

	QObject::connect(ui->widthSpinBox, (void(QSpinBox::*)(int))  &QSpinBox::valueChanged, [this](int value) {
		ui->bombSpinBox->setMaximum(getMaxBomb(ui->widthSpinBox->value(), ui->heightSpinBox->value()));
		});

	QObject::connect(ui->heightSpinBox, (void(QSpinBox::*)(int)) &QSpinBox::valueChanged, [this](int value) {
		ui->bombSpinBox->setMaximum(getMaxBomb(ui->widthSpinBox->value(), ui->heightSpinBox->value()));
		});

	QObject::connect(ui->themeComboBox, (void(QComboBox::*)(int)) &QComboBox::activated, [](int value) {
		ResourceManager::get().changeTheme(IconTheme(value));
		Settings::get().icon_theme = IconTheme(value);
		});

	QObject::connect(ui->backgroundMusicVolumeSlider, &QSlider::valueChanged, [](int value) {
		SoundManager::get().setVolume(BACKGROUND_MUSIC_INDEX, value);
		Settings::get().background_music_volume = value;
		});

	QObject::connect(ui->effectSoundSlider, &QSlider::valueChanged, [](int value) {
		SoundManager::get().setVolume(DUG_INDEX, value);
		SoundManager::get().setVolume(FLAG_INDEX, value);
		SoundManager::get().setVolume(GAME_OVER_INDEX, value);
		SoundManager::get().setVolume(GAME_WIN_INDEX, value);
		Settings::get().effect_sound_volume = value;
		SoundManager::get().play(DUG_INDEX);
		});

	QObject::connect(ui->okButton, &QPushButton::pressed, [this]() {
		Settings::get().width_count = ui->widthSpinBox->value();
		Settings::get().height_count = ui->heightSpinBox->value();
		Settings::get().bomb_count = ui->bombSpinBox->value();
		});
}

PreferencesDialog::~PreferencesDialog() {
	delete ui;
}
