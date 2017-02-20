#include "Weapon.h"

Weapon::Weapon()
{
}
Weapon::~Weapon()
{
	for (int i = 0; i < clipSize; ++i) //delete player bullet
	{
		if (pBullet[i] != 0)
			delete pBullet[i];
	}

}
Weapon::Weapon(string name, Vector3 pos, size_t BulletSpeed) :
GameObject(name, pos), clip(45), canister(0), bulletSpeed(BulletSpeed)
{
	for (int i = 0; i < clipSize; ++i) //Initialize player bullet
		pBullet[i] = 0;
}

unsigned Weapon::canisterLeft()
{
	return (this->canister);
}
unsigned Weapon::bulletLeft()
{
	return (this->clip);
}

void Weapon::pickupClip()
{
	canister++;
}
void Weapon::reload()
{
	clip = clipSize;
}

void Weapon::shoot() //Wait for bullets
{

}
void Weapon::updateBullet(double dt)
{
	for (int i = 0; i < clipSize; ++i)
	{
		//Add shoot here
		//Position.x += (float)(bulletSpeed * sin(Math::DegreeToRadian(rotation)) *dt);
		//Position.z += (float)(bulletSpeed * cos(Math::DegreeToRadian(rotation)) *dt);
	}
}