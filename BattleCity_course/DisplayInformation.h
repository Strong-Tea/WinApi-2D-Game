#pragma once
#include "Player.h"
#include <string>

class DisplayInformation
{
private:
	HBITMAP imageHeart;
	Vectorf imageSize;
	Player* player;
	RECT& windowRect;
public:

	DisplayInformation(LPCWSTR hBitmapPath, RECT& WindowRect, Player* player);

	~DisplayInformation();

	void renderDisplayInformation(HDC& memDC, HDC& memDC1, double fps);

	void renderHelthpoint(HDC& memDC, HDC& memDC1);

	void renderFPS(HDC& memDC, double fps);
};
