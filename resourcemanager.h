#pragma once

#include <QPixmap>
#include <QCache>
#include "minestatus.h"

enum IconTheme {
	ICON_THEME1,
	ICON_THEME2,
	ICON_THEME3,
	ICON_THEME4,
};

class ResourceManager {
public:	
	static ResourceManager& get() {
		static ResourceManager rm;
		return rm;
	}

	QPixmap getStatusImage(MineStatus status);

	QIcon getThemeIcon(IconTheme icon_theme) const;

	void changeTheme(IconTheme change_theme);

	IconTheme getIconTheme() const;

	QPixmap getBombIcon() const;

private:
	ResourceManager();

	void addCache(const QString &url, MineStatus status) const;

	void loadResource();

	IconTheme theme;
	mutable QCache<MineStatus, QPixmap> cache;
};
