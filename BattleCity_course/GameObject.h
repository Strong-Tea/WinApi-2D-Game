#pragma once
#include <Windows.h>
#include "Structs.h"

class Camera;

class GameObject
{
protected:
	Vectorf position;
	SIZE spriteSize;
public:

	GameObject(float positionX, float positionY);
	GameObject(const Vectorf& position);

	virtual ~GameObject();
	virtual void render(HDC& memDC, HDC& memDC1, RECT& windowRect, Camera* camera) = 0;

	const Vectorf& getPos() const;
	float getPosX() const;
	float getPosY() const;

	const SIZE& getSpriteSize() const;
	LONG getSpriteWidth() const;
	LONG getSpriteHeight() const;

	void setPosition(const Vectorf& pos);
	void setPosition(const float x, const float y);
};


class Camera {
public:
	Camera(RECT& WindowRect);
	void setTarget(GameObject* centerObject);
	void update(float deltaTime);
	const Vectorf& getCameraPosition() const;
private:
	int screenWidth_;
	int screenHeight_;
	GameObject* centerObject;
	Vectorf position_;
	RECT& windowRect;
};
