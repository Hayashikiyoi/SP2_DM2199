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
void PlayerBullet::shooting()
{
	if (!isShot)
	{
		isShot = true;
	}
}
void PlayerBullet::updateBullet(double dt, size_t speed)
{
	if (isShot)
	{
		Position += ((speed * dt) * velcocity);
		updateCurPos();
		timer += dt;
		if (timer > 2.5)
		{
			timer = 0;
			isShot = false;
		}
	}
}