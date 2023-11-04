#include "MapTile.h"
#include "Level.h"

MapTile::MapTile(float x, float y, float sizeX, float sizeY, int blockID)
{
	this->x = x;
	this->y = y;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->blockID = blockID;

	rect.left = x;
	rect.top = y;
	rect.right = x + sizeX;
	rect.bottom = y + sizeY;
}

void MapTile::render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera)
{
	//HDC hdcMem = CreateCompatibleDC(memDC);
	HBITMAP hBitmapOld = static_cast<HBITMAP>(SelectObject(memDC1, Level::getBitmapManager()->GetLayerMapImg(blockID - 1)->GetImg()));
	//SelectObject(memDC1, Level::getBitmapManager()->GetLayerMapImg(blockID - 1)->GetImg());
	BitBlt(memDC, (int)(x - camera->getCameraPosition().x), (int)(y - camera->getCameraPosition().y), windowRect.right, windowRect.bottom, memDC1, 0, 0, SRCCOPY);
	SelectObject(memDC1, hBitmapOld);
	//DeleteDC(hdcMem);
}

void MapTile::render(HDC& memDC, HDC& memDC1, Camera* camera)
{
	SelectObject(memDC1, Level::getBitmapManager()->GetLayerMapImg(blockID - 1)->GetImg());
	TransparentBlt(memDC, (int)(x - camera->getCameraPosition().x), (int)(y - camera->getCameraPosition().y), 32, 32, memDC1, 0, 0, 32, 32, RGB(255, 0, 255));
}
