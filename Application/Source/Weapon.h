#ifndef WEAPON_H
#define WEAPON_H

#define clipSize 45

#include "GameObject.h"
#include "Bullet.h"
#include "Mtx44.h"

class Weapon : public GameObject
{
public:
	Weapon();
	Weapon(string name, Vector3 pos, size_t BulletSpeed);
	virtual ~Weapon();

	Mtx44 rotateGunToCamera(Vector3 CameraPos, Vector3 CameraUp, Vector3 CameraTarget);
	void updateBullet(double dt);
	void shoot();
	void reload();
	void pickupClip();
	unsigned bulletLeft();
	unsigned canisterLeft();

private:
	Bullet* pBullet[clipSize];
	size_t clip, canister, bulletSpeed;
	Vector3 up, forward, right;
};

#endif