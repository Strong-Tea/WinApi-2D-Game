#pragma once
#include "MapTile.h"
#include "BitmapManager.h"
#include "Player.h"

class Level
{
private:
	int level;
	int widthLevel;
	int highLevel;
	int countEnemiesInLevel;
	float enemySpawnTime;
	
	std::vector<MapTile*> mapLayer;
	std::vector<MapTile*> objectsLayer;

	static BitmapManager* bitmapManager;

	void loadMap(LPCWSTR filePath, std::vector<MapTile*>& map);

public:

	Level(int level);

	void setLevel(int level);
	int getLevel() { return level; }

	std::vector<MapTile*>& getDynamicObjects() { return objectsLayer; }

	void loadLevel(int level);

	void render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera, Player* player);

	static BitmapManager* getBitmapManager() { return bitmapManager; }
};
