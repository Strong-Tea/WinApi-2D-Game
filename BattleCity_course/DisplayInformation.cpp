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
		16,                       // ������ ������
		0,                        // ������ ������
		0,                        // ���� ������� ������
		0,                        // ���� �������� ������
		FW_NORMAL,                // ������������ ������
		FALSE,                    // ������
		FALSE,                    // �������������
		FALSE,                    // ������������
		DEFAULT_CHARSET,          // ����� ��������
		OUT_DEFAULT_PRECIS,       // �������� ������
		CLIP_DEFAULT_PRECIS,      // �������� ���������
		DEFAULT_QUALITY,          // �������� ������
		DEFAULT_PITCH | FF_DONTCARE, // ������ �������� � ��������� ������
		L"Verdana"                // ��� ������
	);

	SelectObject(memDC, hFont);

	SetTextColor(memDC, RGB(255, 255, 255));
	SetBkMode(memDC, TRANSPARENT);	

	TextOutW(memDC, 10, 10, wstr.c_str(), wstr.length());

	DeleteObject(hFont);
}
