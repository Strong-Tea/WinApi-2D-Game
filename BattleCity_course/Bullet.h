#pragma once

#include <Windows.h>
#include "GameObject.h"


class Bullet : public GameObject
{
private:
	Vectorf direction;
	float speed;
public:
	Bullet(const Vectorf& position = {});
	virtual ~Bullet();

	// Accessor
	float getDirectionX() const;
	float getDirectionY() const;
	const Vectorf& getDirection() const;

	// Modifiers
	void setDirection(float dirX, float dirY);
	void setDirection(Vectorf& direction);

	//functions
	void move(double deltaTime);
	void render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera);
};
