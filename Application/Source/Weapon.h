#ifndef WEAPON_H
#define WEAPON_H

#define clipSize 150

#include "GameObject.h"
#include "Bullet.h"
#include "Mtx44.h"
#include "playerBullet.h"
#include "Fps_Camera.h"

class Weapon : public GameObject
{
public:
	Weapon();
	Weapon(string name, Vector3 pos, size_t BulletSpeed);
	~Weapon();

	Mtx44 rotateGunToCamera(Vector3 CameraPos, Vector3 CameraUp, Vector3 CameraTarget);
	void updateBullet(double dt);
	void shoot(Fps_Camera *camera);
	void reload();
	void pickupClip();
	unsigned bulletLeft();
	unsigned canisterLeft();

	PlayerBullet* pBullet[clipSize];
private:
	size_t clip, canister, bulletSpeed;
	Vector3 up, forward, right;
	Mtx44 rotation;
	double timer;
};

#endif