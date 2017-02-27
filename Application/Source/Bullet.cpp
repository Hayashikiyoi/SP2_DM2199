#include"Bullet.h"

Bullet::Bullet()
{

}


Bullet::Bullet(string name, Vector3 curpos) :GameObject(name, curpos), shoot(false)
{
}
Bullet::~Bullet()
{

}



void Bullet::shootBullet(float rotate, Vector3 turretPos)
{
	if (!shoot)
	{
		rotation = rotate;
		Position = turretPos;
		shoot = true;
	}
}

void Bullet::bulletUpdate(double dt)
{
	if (shoot)
	{
		Bullet::Position.x -= (float)(150 * sin(Math::DegreeToRadian(rotation))*dt);
		Bullet::Position.z -= (float)(150 * cos(Math::DegreeToRadian(rotation))*dt);
	}
}



