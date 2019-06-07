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

	QPixmap getStatusImage(MineStatus status) const;

	void changeTheme(IconTheme theme) const;

private:
	ResourceManager();

	void addCache(const QString &url, MineStatus status) const;

	void loadResource() const;

	IconTheme theme;
	mutable QCache<MineStatus, QPixmap> cache;
};
