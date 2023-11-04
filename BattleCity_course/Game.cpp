#define NOMINMAX
#include "Game.h"
#include <fstream>
#include <string>
#include <tchar.h>
#include <algorithm>
#pragma comment(lib, "winmm.lib")
#pragma warning (disable:4996)



RECT Game::windowRect;


LRESULT Game::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		GetClientRect(hwnd, &windowRect);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



void Game::initWindow(int screenWidth, int screenHeight, PCWSTR lpWindowName, DWORD dwStyle)
{
	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"MainWindow";
	wc.hCursor = LoadCursorW(NULL, IDC_CROSS);

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		NULL,
		L"MainWindow",
		lpWindowName,
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		screenWidth,
		screenHeight,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Failed to create a window", L"ERROR", MB_OK);
		exit(-1);
	}

	dc = GetDC(hwnd);

	GetClientRect(hwnd, &windowRect);

	ShowWindow(hwnd, SW_SHOWNORMAL);
}


void Game::initGameInformation()
{
	displayInfo = new DisplayInformation(L"images\\git.bmp", windowRect, player);
}


void Game::initPlayer()
{
	player = new Player();
	playerFireTime = 1;
	playerFireElapsedTime = 0;
	playerCanFire = TRUE;
}


void Game::initEnemies()
{
	enemySpawnTime = 10;
	maxEnemies = 10;
	enemySpawnElapsedTime = 0;
	enemyShouldSpawn = TRUE;
}

void Game::initMap()
{
	level = new Level(1);
	level->loadLevel(1);
}

void Game::initCamera()
{
	cameraOffset = new Camera(windowRect);
	cameraOffset->setTarget(player);
}


Game::Game(int screenWidth, int screenHeight)
{
	initWindow(screenWidth, screenHeight, L"Battle City", WS_OVERLAPPEDWINDOW);
	initMap();
	initPlayer();
	initCamera();
	initEnemies();
	initGameInformation();

	srand(time(NULL));
}


void Game::resumeGame()
{
	delete player;
	initPlayer();

	//Delete bullets
	for (auto& bullet : bullets)
	{
		delete bullets.at(0);
		bullets.erase(bullets.begin());
	}

	//Delete enemies
	for (auto& enemy : enemies)
	{
		delete enemies.at(0);
		enemies.erase(enemies.begin());
	}


	// Reload map
	//loadMap(gameLevel);
}

Game::~Game()
{	
}


void Game::addBullet(int Mouse_X, int Mouse_Y)
{
	Bullet* bullet = new Bullet();
	bullet->setPosition(
		player->getPos().x + (float)player->getSpriteWidth() / 2 - (float)bullet->getSpriteWidth() / 2,
		player->getPos().y + (float)player->getSpriteHeight() / 2 - (float)bullet->getSpriteHeight() / 2
	);

	bullets.push_back(bullet);

	Vectorf xyLen = Vectorf(
		Mouse_X - abs(bullet->getPos().x - cameraOffset->getCameraPosition().x),
		Mouse_Y - abs(bullet->getPos().y - cameraOffset->getCameraPosition().y)
	);
	float dxy = sqrt(xyLen.x * xyLen.x + xyLen.y * xyLen.y);
	bullet->setDirection(xyLen.x / dxy, xyLen.y / dxy);
}


void Game::updateEnemies(double deltaTime)
{
	enemySpawnElapsedTime += deltaTime;
	if (enemySpawnElapsedTime >= enemySpawnTime)
	{
		enemyShouldSpawn = TRUE;
		enemySpawnElapsedTime = 0.0;
	}

	if (enemies.size() < maxEnemies && enemyShouldSpawn)
	{
		int random_position = rand() % 5 + 1;
		float rand_x, rand_y;
		switch (random_position)
		{
		case 1: rand_x = 700.f; rand_y = 700.f; break;
		case 2: rand_x = 1430.f; rand_y = 200.f; break;
		case 3: rand_x = 200.f; rand_y = 150.f; break;
		case 4: rand_x = 200.f; rand_y = 715.f; break;
		case 5: rand_x = 1400.f; rand_y = 715.f; break;
		}
		enemies.push_back(new Enemy(Vectorf(rand_x, rand_y)));
		enemyShouldSpawn = FALSE;
	}

	// Update enemies direction
	for (int i = 0; i < enemies.size(); i++)
	{
		Vectorf xyLen = Vectorf(player->getPos().x - enemies[i]->getPos().x, player->getPos().y - enemies[i]->getPos().y);
		float dxy = sqrt(xyLen.x * xyLen.x + xyLen.y * xyLen.y);
		enemies[i]->setDirection(xyLen.x / dxy, xyLen.y / dxy);
	}

	/*char msgbuf[256];
	sprintf(msgbuf, "My variable is %d\n", enemies.size());
	OutputDebugStringA(msgbuf);*/
}



void Game::run()
{
	// Initialize previous time to current time
	DWORD prevTime = timeGetTime();

	// Initialize frame counter and FPS variables
	int frameCount = 0;

	// Initialize timer variables
	DWORD timer = timeGetTime();
	int elapsedTime = 0;

	MSG msg = { };
	while (true)
	{

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Exit
			if (msg.message == WM_QUIT)
				break;
			
			// Add Bullets
			if (msg.message == WM_LBUTTONDOWN)
				if (player->getPlayerCanFire()) {
					addBullet(LOWORD(msg.lParam), HIWORD(msg.lParam));
					player->setPlayerCanFire(FALSE);
					player->setPlayerFireElapsedTime(0);
				}
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Get current time
			DWORD currentTime = timeGetTime();

			// Calculate difference between current time and previous time
			DWORD deltaTime = currentTime - prevTime;

			// If difference is less than desired time interval, sleep for the difference
			if (deltaTime < FRAME_INTERVAL) {
				Sleep(FRAME_INTERVAL - deltaTime);
			}

			prevTime = currentTime;

			// convert to seconds
			update(deltaTime / 1000.0);

			render();

			// Increment frame counter
			frameCount++;

			// Check if timer interval has elapsed
			elapsedTime = currentTime - timer;
			if (elapsedTime >= 1000) {
				// Calculate FPS
				fps = frameCount * 1000 / elapsedTime;

				// Reset timer and frame counter
				timer = currentTime;
				frameCount = 0;
			}
		}
	}
}

void Game::update(double deltaTime)
{
	updatePlayerDirection();
	updateBullets(deltaTime);
	//updateEnemies(deltaTime);
	updateCollision(deltaTime);

	player->updateFire(deltaTime);
	cameraOffset->update(deltaTime);
}


void Game::updatePlayerDirection()
{
	float dirX = 0.f;
	float dirY = 0.f;

	if (GetKeyState('W') < 0) dirY = -1.f;
	if (GetKeyState('A') < 0) dirX = -1.f;
	if (GetKeyState('S') < 0) dirY =  1.f;	
	if (GetKeyState('D') < 0) dirX =  1.f;

	//speed reduction by 30% on each axis if the player moves diagonally
	if (dirX != 0.f && dirY != 0.f)
		player->setDirection(dirX * 0.7f, dirY * 0.7f);
	else
		player->setDirection(dirX, dirY);
}


void Game::updateBullets(double deltaTime)
{
	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->move(deltaTime);
}


BOOL Game::checkCollisionObjects(const GameObject* obj1, const GameObject* obj2) {

	float leftObj1 = obj1->getPosX();
	float topObj1 = obj1->getPosY();
	float rightObj1 = leftObj1 + obj1->getSpriteWidth();
	float bottomObj1 = topObj1 + obj1->getSpriteHeight();

	float leftObj2 = obj2->getPosX();
	float topObj2 = obj2->getPosY();
	float rightObj2 = leftObj2 + obj2->getSpriteWidth();
	float bottomObj2 = topObj2 + obj2->getSpriteHeight();

	if (rightObj1 > leftObj2 && leftObj1 < rightObj2
		&& bottomObj1 > topObj2 && topObj1 < bottomObj2) {

		float left = std::max(leftObj1, leftObj2);
		float right = std::min(rightObj1, rightObj2);
		float top = std::max(topObj1, topObj2);
		float bottom = std::min(bottomObj1, bottomObj2);

		// Collision Info
		collisionInfo.intersectionX = left;
		collisionInfo.intersectionY = top;
		collisionInfo.depthX = right - left;
		collisionInfo.depthY = bottom - top;

		return TRUE;
	}
	else
		return FALSE;
}



void Game::updateCollision(double deltaTime)
{	
	/*for (int i = 0; i < enemies.size(); ++i)
	{
		for (int j = i + 1; j < enemies.size(); ++j)
		{
			if (checkCollisionRect(enemies[i]->getSpriteRect(), enemies[j]->getSpriteRect()))
			{
				Vectorf iPos = enemies[i]->getPos();
				Vectorf jPos = enemies[j]->getPos();
				//enemies[i]->setPosition(enemyPos.x)
				int deltaX = iPos.x - jPos.x;
				int deltaY = iPos.y - jPos.y;

				// Normalize the direction vector
				float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);
				float directionX = deltaX / magnitude;
				float directionY = deltaY / magnitude;

				int impulseForce = 1;
				// Apply impulse force to the enemies in opposite directions
			
				int velocityX = 2.5 + impulseForce * directionX;
				int velocityY = 2.5 + impulseForce * directionY;
				enemies[i]->setPosition(iPos.x + velocityX, iPos.y + velocityY);
				velocityX = 2.5 - impulseForce * directionX;
				velocityY = 2.5 -impulseForce * directionY;
				enemies[j]->setPosition(jPos.x + velocityX, jPos.y + velocityY);
			}
		}
	}*/


	// moving objects along the X axis
	player->move(player->getDirectionX(), 0, deltaTime);
	for (auto& enemy : enemies)
		enemy->move(enemy->getDirectionX(), 0, deltaTime);


	// Checking the X axis intersection
	int counterMap = 0;
	int dxPlayerSign = player->getDirectionX() > 0 ? 1 : -1;
	
	/*for (auto tile_it = level->getDynamicObjects().begin(); tile_it != level->getDynamicObjects().end();)
	{		
		bool tile_removed = false;
		// Player and map
		if (checkCollisionObjects(player, *tile_it)) {
			player->setPosition(player->getPosX() - collisionInfo.depthX * dxPlayerSign, player->getPosY());
		}

		int counterEnemy = 0;
		for (auto& enemy : enemies)
		{
			// Enemy and map
			int dxEnemySign = enemy->getDirectionX() > 0 ? 1 : -1;
			if (checkCollisionObjects(enemy, *tile_it)) {
				enemy->setPosition(enemy->getPosX() - collisionInfo.depthX * dxEnemySign, enemy->getPosY());
			}
		}	
		
		if (!tile_removed) {
			++tile_it;
		}
	}*/


	// moving objects along the Y axis
	player->move(0, player->getDirectionY(), deltaTime);
	for (auto& enemy : enemies)
		enemy->move(0, enemy->getDirectionY(), deltaTime);



	// checking the Y axis intersection
	/*int dyPlayerSign = player->getDirectionY() > 0 ? 1 : -1;
	for (auto& tile : level->getDynamicObjects()) {

		// player and map
		if (checkCollisionObjects(player, tile)) {
			player->setPosition(player->getPosX(), player->getPosY() - collisionInfo.depthY * dyPlayerSign);
		}

		// enemy and map
		for (auto& enemy : enemies)
		{
			int dyEnemySign = enemy->getDirectionY() > 0 ? 1 : -1;
			if (checkCollisionObjects(enemy, tile)) {
				enemy->setPosition(enemy->getPosX(), enemy->getPosY() - collisionInfo.depthY * dyEnemySign);
			}
		}
	}*/


	// Enemies and Bullets
	for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();)
	{
		bool enemy_removed = false;
		for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();)
		{
			if (checkCollisionObjects(*enemy_it, *bullet_it)) 
			{
				delete *enemy_it;
				delete *bullet_it;
				enemy_it = enemies.erase(enemy_it);
				bullet_it = bullets.erase(bullet_it);
				enemy_removed = true;
				break; 
			}
			else {
				++bullet_it;
			}
		}
		if (!enemy_removed) {
			++enemy_it;
		}
	}

	
	/*if (kit != nullptr)
	if (playerPos.x + 33 > kit->pos_x && playerPos.x < kit->pos_x + 33
			&& playerPos.y + 25 > kit->pos_y && playerPos.y < kit->pos_y + 25)
	{
		delete kit;
		kit = nullptr;
		player->addHp(1);
	}

	

	// Enemies and Bullets
	// Enemies and Player
	int counterEnemy = 0;
	Vectorf playerNewPos = player->getPos();
	for (auto& enemy : enemies)
	{
		Vectorf enemyPos = enemy->getPos();
		int counterBullet = 0;

		if (kit!=nullptr)
		if (enemyPos.x + 33 > kit->pos_x && enemyPos.x < kit->pos_x + 33
			&& enemyPos.y + 25 > kit->pos_y && enemyPos.y < kit->pos_y + 25)
		{
			delete kit;
			kit = nullptr;
		}

		if (enemyPos.x + 33 > playerNewPos.x && enemyPos.x < playerNewPos.x + 33
			&& enemyPos.y + 25 > playerNewPos.y && enemyPos.y < playerNewPos.y + 25)
		{
			player->loseHp(1);
			delete enemies.at(counterEnemy);
			enemies.erase(enemies.begin() + counterEnemy);
			--counterEnemy;
			break;
		}
		else
		for (auto& bullet : bullets)
		{
			Vectorf bulletPos = bullet->getPos();

			if (kit != nullptr)
			if (bulletPos.x + 33 > kit->pos_x && bulletPos.x < kit->pos_x + 33
				&& bulletPos.y + 25 > kit->pos_y && bulletPos.y < kit->pos_y + 25)
			{
				delete kit;
				kit = nullptr;
			}

			if (bulletPos.x + 20 > enemyPos.x && bulletPos.x < enemyPos.x + 33
				&& bulletPos.y + 20 > enemyPos.y && bulletPos.y < enemyPos.y + 25)
			{
				delete bullets.at(counterBullet);
				delete enemies.at(counterEnemy);
				bullets.erase(bullets.begin() + counterBullet);
				enemies.erase(enemies.begin() + counterEnemy);
				--counterBullet;
				--counterEnemy;
				player->addKills(1);
				break;
			}
			++counterBullet;
		}
		++counterEnemy;
	}*/
}

void Game::render()
{
	//virtual context for drawing in memory
	HDC memDC = CreateCompatibleDC(dc);
	HDC memDC1 = CreateCompatibleDC(dc);
	HBITMAP memBM = CreateCompatibleBitmap(dc, windowRect.right, windowRect.bottom);

	SelectObject(memDC, memBM);
	SelectObject(memDC1, memBM);

	// Map render
	level->render(memDC, memDC1, windowRect, cameraOffset, player);

	// Bullets render
	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->render(memDC, memDC1, windowRect, cameraOffset);

	// Enemies render
	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->render(memDC, memDC1, windowRect, cameraOffset);

	// Player render
	player->render(memDC, memDC1, windowRect, cameraOffset);
	
		
	// Display information render
	displayInfo->renderDisplayInformation(memDC, memDC1, fps);
	
	BitBlt(dc, 0, 0, windowRect.right, windowRect.bottom, memDC, 0, 0, SRCCOPY);

	DeleteDC(memDC1);
	DeleteDC(memDC);
	DeleteObject(memBM);
	
}