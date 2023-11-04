#include "Player.h"
#include <iostream>
#include "Level.h"
#include <chrono>

Player::Player(const Vectorf& position) :
	GameObject(position)
{
	spriteSize = Level::getBitmapManager()->GetPlayerImg(0)->GetImgSize();

	speed = 220.f;				// speed in pixels per second
	healthPointMax = 3;
	healthPoint = healthPointMax;
	countKills = 0;
	needNewGame = FALSE;

	playerCanFire = TRUE;
	playerFireTime = 1;			// seconds
	playerFireElapsedTime = 0;
	state = 0;
}


Player::~Player()
{

}



int Player::getHp() const
{
	return healthPoint;
}


int Player::getHpMax() const
{
	return healthPointMax;
}


int Player::getCountKills() const
{
	return countKills;
}


float Player::getDirectionX() const
{
	return direction.x;
}


float Player::getDirectionY() const
{
	return direction.y;
}


const Vectorf& Player::getDirection() const
{
	return direction;
}

BOOL Player::getPlayerCanFire() const
{
	return playerCanFire;
}

double Player::getPlayerFireElapsedTime() const
{
	return playerFireElapsedTime;
}


void Player::setDirection(float dirX, float dirY)
{
	direction.x = dirX;
	direction.y = dirY;
}


void Player::setDirection(Vectorf direction)
{
	this->direction = direction;
}


void Player::setHp(const int hp)
{
	healthPoint = hp;
}


void Player::loseHp(const int value)
{
	healthPoint -= value;
}

void Player::setPlayerCanFire(BOOL playerCanFire)
{
	this->playerCanFire = playerCanFire;
}

void Player::setPlayerFireElapsedTime(double playerFireElapsedTime)
{
	this->playerFireElapsedTime = playerFireElapsedTime;
}


void Player::addHp(const int hp)
{
	healthPoint += hp;
}


void Player::addKills(const int kills)
{
	countKills += kills;
}


void Player::move(const float dirX, const float dirY, const double deltaTime)
{
	position.x += dirX * speed * deltaTime;
	position.y += dirY * speed * deltaTime;
}


void Player::updateFire(float time)
{
	playerFireElapsedTime += time;

	if (playerFireElapsedTime >= playerFireTime)
	{
		playerCanFire = TRUE;
		playerFireElapsedTime = 0.0;
	}
}

double GetCurrentTimeInSeconds()
{
	using namespace std::chrono;
	return duration_cast<duration<double>>(system_clock::now().time_since_epoch()).count();
}


void Player::render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera)
{
	static int image_changing = 0;
	static unsigned int frame_count = 0;
	static double last_frame_time = 0.0;
	const double frame_duration = 1.0 / 20.0;  // ????????? ????????????????? ????? (30 ?????? ? ???????)

	// ????????? ???????? ???????
	double current_time = GetCurrentTimeInSeconds();

	// ?????????? ???????, ?????????? ? ??????????? ?????
	double elapsed_time = current_time - last_frame_time;

	// ???? ?????? ?????????? ??????? ??? ????? ????????
	if (elapsed_time >= frame_duration)
	{
		frame_count++;
		last_frame_time = current_time;
	}


	//HDC hdcMem = CreateCompatibleDC(memDC);
	static double angle = 0;
	HBITMAP hrotate = Level::getBitmapManager()->GetPlayerImg(state)->RotateHBitmap(Level::getBitmapManager()->GetPlayerImg(state)->GetImg(), 0);

	BITMAP bit;
	GetObject(hrotate, sizeof(BITMAP), &bit);

	HBITMAP hBitmapOld = (HBITMAP)SelectObject(memDC1, hrotate);

	int drawX = position.x - camera->getCameraPosition().x - bit.bmWidth / 2;
	int drawY = position.y - camera->getCameraPosition().y - bit.bmHeight / 2;
	TransparentBlt(memDC, drawX, drawY, bit.bmWidth, bit.bmHeight, memDC1, 0, 0, bit.bmWidth, bit.bmHeight, RGB(255, 0, 255));
	//BitBlt(memDC, position.x - camera->getCameraPosition().x, position.y - camera->getCameraPosition().y, windowRect.right, windowRect.bottom, memDC1, 0, 0, SRCCOPY);


	////////////////////////////////////////////////////
	HBITMAP hrotate1 = Level::getBitmapManager()->GetPlayerImg(state + 1)->RotateHBitmap(Level::getBitmapManager()->GetPlayerImg(state + 1)->GetImg(), 40);

	GetObject(hrotate1, sizeof(BITMAP), &bit);

	HBITMAP hBitmapOld1 = (HBITMAP)SelectObject(memDC1, hrotate1);

	drawX = drawX + 112 - bit.bmWidth / 2;
	drawY = drawY + 112 - bit.bmHeight / 2;
	TransparentBlt(memDC, drawX, drawY, bit.bmWidth, bit.bmHeight, memDC1, 0, 0, bit.bmWidth, bit.bmHeight, RGB(155, 0, 255));
	///////////////////////////////////////////////////

	SelectObject(memDC1, hBitmapOld);
	SelectObject(memDC1, hBitmapOld1);
	//DeleteDC(hdcMem);
	if (state == 2)
		state = 0;

	if (angle > 360)
		angle = 1;
}
