#ifndef WEAPON_H
#define WEAPON_H

#define clipSize 45

#include "GameObject.h"
#include "Bullet.h"

class Weapon : public GameObject
{
public:
	Weapon();
	Weapon(string name, Vector3 pos, size_t BulletSpeed);
	virtual ~Weapon();

	void rotateGunToCamera(Vector3 CameraPos);
	void updateBullet(double dt);
	void shoot();
	void reload();
	void pickupClip();
	unsigned bulletLeft();
	unsigned canisterLeft();

private:
	Bullet* pBullet[clipSize];
	size_t clip, canister, bulletSpeed;
};

#endif