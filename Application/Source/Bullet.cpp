#include"Bullet.h"

Bullet::Bullet()
{

}

Bullet::~Bullet()
{

}


void Bullet::shootBullet(bool &checking, float rotate, double &dt, Vector3& turretPos)
{
	if (checking)
	{
		Bullet::position.x -= (float)(10 * sin(Math::DegreeToRadian(rotate))*dt);
		Bullet::position.z -= (float)(10 * cos(Math::DegreeToRadian(rotate))*dt);
	}
	if (checking == false)
	{
		Bullet::position.x = turretPos.x;
		Bullet::position.z = turretPos.y;
	}
}