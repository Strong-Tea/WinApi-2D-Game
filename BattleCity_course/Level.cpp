#include "Level.h"
#include <sstream>
#include <fstream>
#include <string>
#pragma warning(disable:4996)

BitmapManager* Level::bitmapManager;

void Level::loadMap(LPCWSTR filePath, std::vector<MapTile*>& map)
{
	/*HANDLE hFile;
	DWORD dwFileSize, dwBytesRead;
	char* pBuffer;


	hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"Level::loadMap::Failed to open the file", L"ERROR", MB_OK);
		return;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		MessageBox(NULL, L"Level::loadMap::Failed to get the file size", L"ERROR", MB_OK);
		CloseHandle(hFile);
		return;
	}

	pBuffer = new char[dwFileSize];

	if (!ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, NULL)) {
		MessageBox(NULL, L"Level::loadMap::Failed to read data from the file", L"ERROR", MB_OK);
		CloseHandle(hFile);
		delete[] pBuffer;
		return;
	}

	CloseHandle(hFile);

	int width, height, tilewidth, tileheight;
	sscanf(pBuffer, "width=\"%d\" height=\"%d\" tilewidth=\"%d\" tileheight=\"%d\"\n", &width, &height, &tilewidth, &tileheight);

	char* token;
	token = strtok(pBuffer, "\n"); // skip first line
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			token = strtok(NULL, ","); // next element
			int blockID = atoi(token);

			if (blockID != 0)
			{
				TCHAR blockPath[356];
				//wsprintf(blockPath, TEXT("images//block%d.bmp"), blockID);
				wsprintf(blockPath, TEXT("images//blackYel.bmp"));

				map.push_back(new MapTile(blockPath, Vectorf(i * tileheight, j * tilewidth), blockID));
				memset(blockPath, 0, sizeof(blockPath));
			}
		}
	}*/

	int width, height, tilewidth, tileheight;

	std::ifstream file(filePath);

	// считываем размеры карты из первой строки
	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss.ignore(256, '\"');
	ss >> width;
	ss.ignore(256, '\"');
	ss.ignore(256, '\"');
	ss >> height;
	ss.ignore(256, '\"');
	ss.ignore(256, '\"');
	ss >> tilewidth;
	ss.ignore(256, '\"');
	ss.ignore(256, '\"');
	ss >> tileheight;

	for (int i = 0; i < height; i++)
	{
		std::getline(file, line);
		std::stringstream ss(line);
		for (int j = 0; j < width; j++)
		{
			std::string element;
			std::getline(ss, element, ',');
			int blockID = std::stoi(element);

			map.push_back(new MapTile(j * tilewidth, i * tileheight, tilewidth, tileheight, blockID));
		}
	}

	file.close();
}

Level::Level(int level)
{
	this->level = level;
	
	countEnemiesInLevel = 20;
	enemySpawnTime = 5.f;

	bitmapManager = new BitmapManager();
	bitmapManager->ImagesLoad(L"..\\images");
}

void Level::setLevel(int level)
{
	this->level = level;
}


void Level::loadLevel(int level)
{
	TCHAR path1[256];
	TCHAR path2[128];
	wsprintf(path1, TEXT("..\\Levels\\level%d\\main_layer.txt"), level);
	wsprintf(path2, TEXT("..\\Levels\\level%d\\layer2.txt"), level);

	loadMap(path1, mapLayer);
	loadMap(path2, objectsLayer);
}

void Level::render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera, Player* player)
{
	int halfScreenWidth = windowRect.right / 2;
	int halfScreenHeight = windowRect.bottom / 2;
	int screenLeft = player->getPosX() - halfScreenWidth - 96;
	int screenTop = player->getPosY() - halfScreenHeight - 96;
	int screenRight = player->getPosX() + halfScreenWidth + 96;
	int screenBottom = player->getPosY() + halfScreenHeight + 96;


	for (int i = 0; i < mapLayer.size(); i++) {
		if ((int)mapLayer[i]->getX() > screenLeft && (int)mapLayer[i]->getX() < screenRight && (int)mapLayer[i]->getY() > screenTop && (int)mapLayer[i]->getY() < screenBottom) {
			mapLayer[i]->render(memDC, memDC1, windowRect, camera);
		}
	}

	for (int i = 0; i < objectsLayer.size(); i++) {		
		objectsLayer[i]->render(memDC, memDC1, camera);
	}
}
