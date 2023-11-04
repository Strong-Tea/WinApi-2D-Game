#pragma once
#include <Windows.h>

class Bitmap
{
private:
	HBITMAP hBitMap;
	SIZE imageSize;
public:
	Bitmap(LPCWSTR hBitmapPath);
	~Bitmap();

	void ImgLoad(LPCWSTR hBitmapPath);

	HBITMAP GetImg() { return hBitMap; }
	HBITMAP RotateHBitmap(HBITMAP hBitmap, double angle);
	HBITMAP RotateHBitmapAroundPoint(HBITMAP hBitmap, double angle, int cxCenter, int cyCenter);
	SIZE GetImgSize() { return imageSize; }
};
