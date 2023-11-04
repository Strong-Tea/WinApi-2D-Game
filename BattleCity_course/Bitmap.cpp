#include "Bitmap.h"
#include <iostream>


Bitmap::Bitmap(LPCWSTR hBitmapPath)
{
	ImgLoad(hBitmapPath);
}

Bitmap::~Bitmap()
{
	DeleteObject(hBitMap);
}


void Bitmap::ImgLoad(LPCWSTR hBitmapPath)
{
	hBitMap = (HBITMAP)LoadImage(NULL, hBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bit;
	GetObject(hBitMap, sizeof(BITMAP), &bit);
	imageSize.cx = bit.bmWidth;
	imageSize.cy = bit.bmHeight;
}

HBITMAP Bitmap::RotateHBitmap(HBITMAP hBitmap, double angle)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	// Calculating the new dimensions of the rotated image
	double radians = angle * 3.14159265358979323846 / 180.0;
	int newWidth = static_cast<int>(fabs(bitmap.bmWidth * cos(radians)) + fabs(bitmap.bmHeight * sin(radians)));
	int newHeight = static_cast<int>(fabs(bitmap.bmWidth * sin(radians)) + fabs(bitmap.bmHeight * cos(radians)));

	// Creating a device context for the source image
	HDC hdcSrc = CreateCompatibleDC(NULL);
	SelectObject(hdcSrc, hBitmap);

	// Creating a device context for a new image
	HDC hdcDest = CreateCompatibleDC(NULL);

	// Creating a new HBITMAP with dimensions corresponding to the rotated image
	HBITMAP hBitmapRotated = CreateCompatibleBitmap(hdcSrc, newWidth, newHeight);
	SelectObject(hdcDest, hBitmapRotated);

	// Fill the background of the new image with white
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
	RECT rect = { 0, 0, newWidth, newHeight };
	FillRect(hdcDest, &rect, hBrush);

	// Performing image rotation
	int cxSrc = bitmap.bmWidth / 2;
	int cySrc = bitmap.bmHeight / 2;
	int cxDest = newWidth / 2;
	int cyDest = newHeight / 2;
	SetGraphicsMode(hdcDest, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = static_cast<FLOAT>(cos(radians));
	xform.eM12 = static_cast<FLOAT>(sin(radians));
	xform.eM21 = static_cast<FLOAT>(-sin(radians));
	xform.eM22 = static_cast<FLOAT>(cos(radians));
	xform.eDx = static_cast<FLOAT>(cxDest - cxSrc * cos(radians) + cySrc * sin(radians));
	xform.eDy = static_cast<FLOAT>(cyDest - cxSrc * sin(radians) - cySrc * cos(radians));
	SetWorldTransform(hdcDest, &xform);

	// Copy the original image to the new image
	BitBlt(hdcDest, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY);

	DeleteObject(hBrush);
	DeleteDC(hdcSrc);
	DeleteDC(hdcDest);

	return hBitmapRotated;
}

HBITMAP Bitmap::RotateHBitmapAroundPoint(HBITMAP hBitmap, double angle, int cxCenter, int cyCenter)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	// Calculating the new dimensions of the rotated image
	double radians = angle * 3.14159265358979323846 / 180.0;
	int newWidth = static_cast<int>(fabs(bitmap.bmWidth * cos(radians)) + fabs(bitmap.bmHeight * sin(radians)));
	int newHeight = static_cast<int>(fabs(bitmap.bmWidth * sin(radians)) + fabs(bitmap.bmHeight * cos(radians)));

	// Creating a device context for the source image
	HDC hdcSrc = CreateCompatibleDC(NULL);
	SelectObject(hdcSrc, hBitmap);

	// Creating a device context for a new image
	HDC hdcDest = CreateCompatibleDC(NULL);

	// Creating a new HBITMAP with dimensions corresponding to the rotated image
	HBITMAP hBitmapRotated = CreateCompatibleBitmap(hdcSrc, newWidth, newHeight);
	SelectObject(hdcDest, hBitmapRotated);

	// Fill the background of the new image with white
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // White background
	RECT rect = { 0, 0, newWidth, newHeight };
	FillRect(hdcDest, &rect, hBrush);

	// Performing image rotation around the specified center
	SetGraphicsMode(hdcDest, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = static_cast<FLOAT>(cos(radians));
	xform.eM12 = static_cast<FLOAT>(sin(radians));
	xform.eM21 = static_cast<FLOAT>(-sin(radians));
	xform.eM22 = static_cast<FLOAT>(cos(radians));
	xform.eDx = static_cast<FLOAT>(cxCenter - cxCenter * cos(radians) + cyCenter * sin(radians));
	xform.eDy = static_cast<FLOAT>(cyCenter - cxCenter * sin(radians) - cyCenter * cos(radians));
	SetWorldTransform(hdcDest, &xform);

	// Copy the original image to the new image
	BitBlt(hdcDest, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY);

	DeleteObject(hBrush);
	DeleteDC(hdcSrc);
	DeleteDC(hdcDest);

	return hBitmapRotated;
}


