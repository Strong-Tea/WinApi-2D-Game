#include "DisplayInformation.h"
#pragma comment(lib, "Msimg32.lib")

DisplayInformation::DisplayInformation(LPCWSTR hBitmapPath, RECT& WindowRect, Player* player) : windowRect(WindowRect)
{
	imageHeart = (HBITMAP)LoadImage(NULL, hBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	this->player = player;

	BITMAP bitmap;
	GetObject(imageHeart, sizeof(BITMAP), &bitmap);
	imageSize.x = bitmap.bmWidth;
	imageSize.y = bitmap.bmHeight;
}

DisplayInformation::~DisplayInformation()
{
	if (imageHeart != NULL)
	{
		DeleteObject(imageHeart);
		imageHeart = NULL;
	}
}

void DisplayInformation::renderDisplayInformation(HDC& memDC, HDC& memDC1, double fps)
{
	renderHelthpoint(memDC, memDC1);
	renderFPS(memDC, fps);
}


void DisplayInformation::renderHelthpoint(HDC& memDC, HDC& memDC1)
{
	SelectObject(memDC1, imageHeart);

	int distanceSpace = 5;
	int x = windowRect.right - imageSize.x - distanceSpace;;
	int y = windowRect.top + 10;

	for (int i = 0; i < player->getHp(); i++)
	{
		//SetBkColor(memDC1, RGB(0, 0, 0));
		TransparentBlt(memDC, x, y, 20, 20, memDC1, 0, 0, 20, 20, RGB(255, 0, 255));
		//TransparentBlt(memDC, x, y, windowRect.right, windowRect.bottom, memDC1, 0, 0, 20, 20, RGB(0, 0, 0));
		//BitBlt(memDC, x, y, windowRect.right, windowRect.bottom, memDC1, 0, 0, SRCCOPY);
		x = x - imageSize.x - distanceSpace;
	}
}


void DisplayInformation::renderFPS(HDC& memDC, double fps)
{
	wchar_t buffer[] = L"FPS: ";
	const std::wstring wstr = buffer + std::to_wstring(fps);


	HFONT hFont = CreateFont(
		16,                       // высота шрифта
		0,                        // ширина шрифта
		0,                        // угол наклона шрифта
		0,                        // угол поворота шрифта
		FW_NORMAL,                // насыщенность шрифта
		FALSE,                    // курсив
		FALSE,                    // подчеркивание
		FALSE,                    // зачеркивание
		DEFAULT_CHARSET,          // набор символов
		OUT_DEFAULT_PRECIS,       // точность вывода
		CLIP_DEFAULT_PRECIS,      // точность отсечения
		DEFAULT_QUALITY,          // качество шрифта
		DEFAULT_PITCH | FF_DONTCARE, // ширина символов и семейство шрифта
		L"Verdana"                // имя шрифта
	);

	SelectObject(memDC, hFont);

	SetTextColor(memDC, RGB(255, 255, 255));
	SetBkMode(memDC, TRANSPARENT);	

	TextOutW(memDC, 10, 10, wstr.c_str(), wstr.length());

	DeleteObject(hFont);
}
