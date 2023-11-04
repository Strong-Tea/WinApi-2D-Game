#include "Enemy.h"
#include "Level.h"


Enemy::Enemy(const Vectorf& position) :
	GameObject(position)
{
	spriteSize = Level::getBitmapManager()->GetEnemyImg(0)->GetImgSize();

	speed = 100.f;
	healthPointMax = 100.f;
	healthPoint = healthPointMax;
}


Enemy::~Enemy()
{
}


int Enemy::getHp() const
{
	return healthPoint;
}


int Enemy::getHpMax() const
{
	return healthPointMax;
}



float Enemy::getDirectionX() const
{
	return direction.x;
}


float Enemy::getDirectionY() const
{
	return direction.y;
}


const Vectorf& Enemy::getDirection() const
{
	return direction;
}


void Enemy::setDirection(float dirX, float dirY)
{
	direction.x = dirX;
	direction.y = dirY;
}


void Enemy::setDirection(Vectorf direction)
{
	this->direction = direction;
	
}


void Enemy::setHp(const int hp)
{
	healthPoint = hp;
}


void Enemy::loseHp(const int value)
{
	healthPoint -= value;
}



void Enemy::move(const float dirX, const float dirY, const double deltaTime)
{
	position.x += dirX * speed * deltaTime;
	position.y += dirY * speed * deltaTime;
}


void Enemy::render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera)
{
	SelectObject(memDC1, Level::getBitmapManager()->GetEnemyImg(0)->GetImg());
	BitBlt(memDC, position.x - camera->getCameraPosition().x, position.y - camera->getCameraPosition().y, windowRect.right, windowRect.bottom, memDC1, 0, 0, SRCCOPY);
}
