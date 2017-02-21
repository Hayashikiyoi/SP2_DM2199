#include "playerBullet.h"

PlayerBullet::PlayerBullet()
{
}
PlayerBullet::PlayerBullet(string name, Vector3 pos) : GameObject(name, pos), isShot(false), timer(0)
{
}
PlayerBullet::~PlayerBullet()
{
}

bool PlayerBullet::shot(void)
{
	return isShot;
}
void PlayerBullet::shooting(bool shot, Mtx44 rotation)
{
	if (!isShot)
	{
		this->rot = rotation;
		isShot = true;
	}
}
void PlayerBullet::updateBullet(double dt)
{
	if (isShot)
	{
		Position += (100 * dt);
		timer += dt;
		if (timer > 2.5)
		{
			timer = 0;
			isShot = false;
		}
	}
}

Mtx44 PlayerBullet::rotMatrix()
{
	return (this->rot);
}