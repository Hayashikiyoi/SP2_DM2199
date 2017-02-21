#include "playerBullet.h"

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
		Position += (10 * dt);
	}
}