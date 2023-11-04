#pragma once
#include <Windows.h>
#include "GameObject.h"

class Player : public GameObject
{
private:
	Vectorf direction;
	BOOL needNewGame;

	float speed;
	int healthPoint;
	int healthPointMax;
	int countKills;
	int state;

	// Shooting
	BOOL playerCanFire;
	double playerFireTime;
	double playerFireElapsedTime;

public:

	Player(const Vectorf& position = {});
	virtual ~Player();

	//Accessor

	int getHp() const;
	int getHpMax() const;
	int getCountKills() const;
	float getDirectionX() const;
	float getDirectionY() const;
	const Vectorf& getDirection() const;
	BOOL getPlayerCanFire() const;
	double getPlayerFireElapsedTime() const;

	//Modifiers
	void setDirection(float dirX, float dirY);
	void setDirection(Vectorf direction);
	void setHp(const int hp);
	void addHp(const int hp);
	void addKills(const int kills);
	void loseHp(const int value);
	void setPlayerCanFire(BOOL playerCanFire);
	void setPlayerFireElapsedTime(double playerFireElapsedTime);
	

	//functions
	void move(const float dirX, const float dirY, const double deltaTime);
	void updateFire(float time);
	void render(HDC& memDC,HDC& memDC1,RECT& windowRect, Camera* camera);
};
