#include <QDebug>
#include "resourcemanager.h"

ResourceManager::ResourceManager()
	: theme(ICON_THEME1) {
	loadResource();	
}

void ResourceManager::loadResource() const {
	changeTheme(theme);
	addCache(":/Resources/Resources/imgs/num1.png", STATUS_NUM1);
	addCache(":/Resources/Resources/imgs/num2.png", STATUS_NUM2);
	addCache(":/Resources/Resources/imgs/num3.png", STATUS_NUM3);
	addCache(":/Resources/Resources/imgs/num4.png", STATUS_NUM4);
	addCache(":/Resources/Resources/imgs/num5.png", STATUS_NUM5);
	addCache(":/Resources/Resources/imgs/num6.png", STATUS_NUM6);
	addCache(":/Resources/Resources/imgs/num7.png", STATUS_NUM7);
	addCache(":/Resources/Resources/imgs/num8.png", STATUS_NUM8);
}

void ResourceManager::changeTheme(IconTheme theme) const {
	cache.remove(STATUS_BOMB);
	cache.remove(STATUS_BANK);
	cache.remove(STATUS_INIT);
	cache.remove(STATUS_FLAG);

	switch (theme) {
	case ICON_THEME1:
		addCache(":/Resources/Resources/imgs/mine.png", STATUS_BOMB);
		addCache(":/Resources/Resources/imgs/bank.png", STATUS_BANK);
		addCache(":/Resources/Resources/imgs/init.png", STATUS_INIT);
		addCache(":/Resources/Resources/imgs/flag.png", STATUS_FLAG);
		break;
	case ICON_THEME2:
		addCache(":/Resources/Resources/imgs/274c.png", STATUS_BOMB);
		addCache(":/Resources/Resources/imgs/2b55.png", STATUS_BANK);
		addCache(":/Resources/Resources/imgs/2716.png", STATUS_INIT);
		addCache(":/Resources/Resources/imgs/2757.png", STATUS_FLAG);
		break;
	case ICON_THEME3:
		addCache(":/Resources/Resources/imgs/26a1.png", STATUS_BOMB);
		addCache(":/Resources/Resources/imgs/2600.png", STATUS_BANK);
		addCache(":/Resources/Resources/imgs/2601.png", STATUS_INIT);
		addCache(":/Resources/Resources/imgs/2614.png", STATUS_FLAG);
		break;
	case ICON_THEME4:
		addCache(":/Resources/Resources/imgs/1f47e.png", STATUS_BOMB);
		addCache(":/Resources/Resources/imgs/1f370.png", STATUS_BANK);
		addCache(":/Resources/Resources/imgs/init.png", STATUS_INIT);
		addCache(":/Resources/Resources/imgs/1f4cc.png", STATUS_FLAG);
		break;
	}
}

void ResourceManager::addCache(const QString& url, MineStatus status) const {
	cache.insert(status, new QPixmap(url));
}

QPixmap ResourceManager::getStatusImage(MineStatus status) const {
	auto obj = cache.object(status);
	if (!obj) {
		loadResource();
		obj = cache.object(status);
		qDebug() << "Load resource!";
	}
	return *obj;
}
