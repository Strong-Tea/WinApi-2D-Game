#pragma once

struct Vectorf
{
	float x, y;

	Vectorf() :x(0.f), y(0.f) {}

	Vectorf(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};


struct Rectf
{
	float left, top, right, bottom;

	Rectf() :left(0.f), top(0.f), right(0.f), bottom(0.f) {}

	Rectf(float left, float top, float right, float bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};

struct CollisionInfo {
	float intersectionX;
	float intersectionY;
	float depthX;
	float depthY;
};

enum MAP
{

};
