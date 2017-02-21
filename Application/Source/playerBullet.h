#ifndef PlAYERBULLET_H
#define PLAYERBULLET_H

#include "GameObject.h"
#include "Mtx44.h"

class PlayerBullet : public GameObject
{
public:
	PlayerBullet();
	PlayerBullet(string name, Vector3 pos);
	virtual ~PlayerBullet();

	//return Boolean for shooting
	bool shot(void);
	//Set boolean and rotaion of current gun for bullet
	void shooting(bool shot, Mtx44 rotation);

	//Update for bullet (Translate bullet in the foward vector)
	void updateBullet(double dt);

	//For load matrix
	Mtx44 rotMatrix();

private:
	bool isShot;
	//Vector3 up, forward, right; //Try to not include first
	Mtx44 rot;
};

PlayerBullet::PlayerBullet()
{
}
PlayerBullet::PlayerBullet(string name, Vector3 pos) : GameObject(name, pos), isShot(false)
{
}
PlayerBullet::~PlayerBullet()
{
}

bool PlayerBullet::shot(void)
{
	return isShot;
}
#endif