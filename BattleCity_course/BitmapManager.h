#pragma once
#include "Bitmap.h"
#include "Structs.h"
#include <vector>
#include <string>

class BitmapManager
{
private:
	std::vector<Bitmap*> layerMap;
	std::vector<Bitmap*> dynamicMap;
	std::vector<Bitmap*> player;
	std::vector<Bitmap*> enemy;
	std::vector<Bitmap*> bullet;

	void removeNumbers(LPWSTR str);
public:

	BitmapManager();
	~BitmapManager();

	void ImagesLoad(LPCWSTR directory);

	void Clear();

	Bitmap* GetLayerMapImg(int index) { return layerMap.at(index); }
	Bitmap* GetPlayerImg(int index) { return player.at(index); }
	Bitmap* GetEnemyImg(int index) { return enemy.at(index); }
	Bitmap* GetBulletImg(int index) { return bullet.at(index); }
};
