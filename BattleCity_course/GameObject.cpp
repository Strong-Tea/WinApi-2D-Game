#include "GameObject.h"


GameObject::GameObject(float positionX, float positionY)
{
	position.x = positionX;
	position.y = positionY;
	spriteSize.cx = 0;
	spriteSize.cy = 0;
}


GameObject::GameObject(const Vectorf& position)
{
	this->position = position;
	spriteSize.cx = 0;
	spriteSize.cy = 0;
}


GameObject::~GameObject()
{
}


const Vectorf& GameObject::getPos() const
{
	return position;
}


float GameObject::getPosX() const
{
	return position.x;
}


float GameObject::getPosY() const
{
	return position.y;
}


void GameObject::setPosition(const Vectorf& pos)
{
	position = pos;
}


void GameObject::setPosition(const float x, const float y)
{
	position.x = x;
	position.y = y;
}


const SIZE& GameObject::getSpriteSize() const
{
	return spriteSize;
}


LONG GameObject::getSpriteWidth() const
{
	return spriteSize.cx;
}


LONG GameObject::getSpriteHeight() const
{
	return spriteSize.cy;
}


Camera::Camera(RECT& WindowRect) : windowRect(WindowRect) {
	//windowRect = WindowRect;
	centerObject = nullptr;
	position_.x = 0;
	position_.y = 0;
}

void Camera::setTarget(GameObject* centerObject) {
	this->centerObject = centerObject;
}


void Camera::update(float deltaTime) {
	if (centerObject != nullptr) {
		POINT desiredPosition = { centerObject->getPosX() - windowRect.right / 2, centerObject->getPosY() - windowRect.bottom / 2 };
		float smoothingFactor = 5.0f;
		position_.x += (desiredPosition.x - position_.x) * (deltaTime * smoothingFactor);
		position_.y += (desiredPosition.y - position_.y) * (deltaTime * smoothingFactor);
	}
}


const Vectorf& Camera::getCameraPosition() const {
	return this->position_;
}