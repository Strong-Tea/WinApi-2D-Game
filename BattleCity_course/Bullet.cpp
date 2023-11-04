#include "Bullet.h"
#include "Level.h"

Bullet::Bullet(const Vectorf& position) :
	GameObject(position)
{
	spriteSize = Level::getBitmapManager()->GetBulletImg(0)->GetImgSize();
	this->speed = 650.f;		// speed in pixels per second
}

Bullet::~Bullet()
{
}

float Bullet::getDirectionX() const
{
	return direction.x;
}


float Bullet::getDirectionY() const
{
	return direction.y;
}


const Vectorf& Bullet::getDirection() const
{
	return direction;
}


void Bullet::setDirection(float dirX, float dirY)
{
	direction.x = dirX;
	direction.y = dirY;
}


void Bullet::setDirection(Vectorf& direction)
{
	this->direction = direction;
}


void Bullet::move(double deltaTime)
{
	position.x += direction.x * speed * deltaTime;
	position.y += direction.y * speed * deltaTime;
}

void Bullet::render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera)
{
	SelectObject(memDC1, Level::getBitmapManager()->GetBulletImg(0)->GetImg());
	BitBlt(memDC, position.x - camera->getCameraPosition().x, position.y - camera->getCameraPosition().y, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, memDC1, 0, 0, SRCCOPY);
}

