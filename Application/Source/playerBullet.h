#ifndef PlAYERBULLET_H
#define PLAYERBULLET_H

#include "GameObject.h"
#include "Mtx44.h"
#include "Fps_Camera.h"

class PlayerBullet : public GameObject
{
public:
	PlayerBullet();
	PlayerBullet(string name, Vector3 pos);
	virtual ~PlayerBullet();

	//return Boolean for shooting
	bool shot(void);
	//Set boolean and rotaion of current gun for bullet
	void shooting();

	//Update for bullet (Translate bullet in the foward vector)
	void updateBullet(double dt, size_t speed);

	//Forward Vector
	Vector3 velcocity;
private:
	bool isShot;
	//Vector3 up, forward, right; //Try to not include first
	Mtx44 rot;
	double timer;
};
#endif