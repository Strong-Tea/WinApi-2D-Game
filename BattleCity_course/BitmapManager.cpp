#include "BitmapManager.h"
#include <string>
#include <algorithm>
#include <regex>

BitmapManager::BitmapManager()
{
}

BitmapManager::~BitmapManager()
{
}


bool compareFileNames(const std::wstring& fileName1, const std::wstring& fileName2) {

    std::wregex numberRegex(L"\\D(\\d+)\\D");

    std::wsmatch match1, match2;
    std::regex_search(fileName1, match1, numberRegex);
    std::regex_search(fileName2, match2, numberRegex);

    int number1 = std::stoi(match1[1].str());
    int number2 = std::stoi(match2[1].str());

    return number1 < number2;
}


void BitmapManager::ImagesLoad(LPCWSTR directory)
{
	TCHAR searchPath[MAX_PATH];
	wsprintf(searchPath, TEXT("%s\\*.%s"), directory, L"bmp");

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            TCHAR fileTypeName[32];
            TCHAR filePath[MAX_PATH];
            wsprintf(fileTypeName, TEXT("%s"), findData.cFileName);
            wsprintf(filePath, TEXT("%s\\%s"), directory, findData.cFileName);

            removeNumbers(fileTypeName);

            if (lstrcmpW(fileTypeName, L"block.bmp") == 0)
            {
                layerMap.push_back(new Bitmap(filePath));
                continue;
            }

            if (lstrcmpW(fileTypeName, L"player.bmp") == 0)
            {
                player.push_back(new Bitmap(filePath));
                continue;
            }

            if (lstrcmpW(fileTypeName, L"enemy.bmp") == 0)
            {
                enemy.push_back(new Bitmap(filePath));
                continue;
            }

            if (lstrcmpW(fileTypeName, L"bullet.bmp") == 0)
            {
                bullet.push_back(new Bitmap(filePath));
                continue;
            }
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}


void BitmapManager::Clear() {
    for (std::vector<Bitmap*>::iterator it = layerMap.begin(); it != layerMap.end(); it++)
        delete (*it);
    layerMap.clear();

    for (std::vector<Bitmap*>::iterator it = player.begin(); it != player.end(); it++)
        delete (*it);
    player.clear();

    for (std::vector<Bitmap*>::iterator it = enemy.begin(); it != enemy.end(); it++)
        delete (*it);
    enemy.clear();

    for (std::vector<Bitmap*>::iterator it = bullet.begin(); it != bullet.end(); it++)
        delete (*it);
    bullet.clear();
}


void BitmapManager::removeNumbers(LPWSTR str) {
    LPWSTR dest = str;
    while (*str) {
        if (!iswdigit(*str)) {
            *dest = *str;
            dest++;
        }
        str++; }
    *dest = '\0';
}
