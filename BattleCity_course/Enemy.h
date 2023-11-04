#pragma once
#include <Windows.h>
#include "GameObject.h"


class Enemy : public GameObject
{
private:
	Vectorf direction;

	float speed;
	int healthPoint, healthPointMax;
	
public:

	Enemy(const Vectorf& position = {});
	virtual ~Enemy();

	// Accessor
	int getHp() const;
	int getHpMax() const;
	float getDirectionX() const;
	float getDirectionY() const;
	const Vectorf& getDirection() const;

	// Modifiers
	void setDirection(float dirX, float dirY);
	void setDirection(Vectorf direction);
	void setHp(const int hp);
	void loseHp(const int value);

	// Functions
	void move(const float dirX, const float dirY, const double deltaTime);
	void render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera);
};
