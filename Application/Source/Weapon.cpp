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
		pBullet[i] = new PlayerBullet("Bullet", pos);
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
	for (int i = 0; i < clipSize; ++i)
	{
		if (!pBullet[i]->shot() && pBullet[i])
		{
			pBullet[i]->shooting(true, rotation);
		}
	}
}
void Weapon::updateBullet(double dt)
{

}

Mtx44 Weapon::rotateGunToCamera(Vector3 CameraPos, Vector3 CameraUp, Vector3 CameraTarget)
{
	forward = (CameraTarget - CameraPos).Normalized();
	up = Vector3(0, 1, 0);
	right = forward.Cross(up);
	right.Normalize();
	right.y = 0;
	up = right.Cross(forward).Normalized();
	rotation = Mtx44(right.x, right.y, right.z, 0, up.x, up.y, up.z, 0, forward.x, forward.y, forward.z, 0, CameraPos.x, CameraPos.y, CameraPos.z, 1);
	return rotation;
}