#include"Bullet.h"

Bullet::Bullet()
{

}


Bullet::Bullet(string name, Vector3 curpos) :GameObject(name, curpos)
{
}
Bullet::~Bullet()
{

}



void Bullet::shootBullet(float rotate, double dt, Vector3 turretPos,bool& shoot)
{
	if (shoot)
	{
	Bullet::Position.x -= (float)(10 * sin(Math::DegreeToRadian(rotate))*dt);
	Bullet::Position.z -= (float)(10 * cos(Math::DegreeToRadian(rotate))*dt);
	}
	

}