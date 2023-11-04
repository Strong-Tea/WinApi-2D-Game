#pragma once
#include "GameObject.h"



class MapTile
{
private:
	float x;
	float y;
	float sizeX;
	float sizeY;

	int blockID;

	RECT rect;
public:

	MapTile(float x, float y, float sizeX, float sizeY, int blockID);

	void render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera);
	void render(HDC& memDC, HDC& memDC1, Camera* camera);
	
	float getX() { return x; }
	float getY() { return y; }
};
