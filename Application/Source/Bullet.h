#ifndef BULLET_H
#define BULLET_H
#include"GameObject.h"

class Bullet : public GameObject
{	
public:
	Bullet();
	Bullet(string name, Vector3 curpos);
	virtual ~Bullet();
	void shootBullet(float rotate, double dt, Vector3 turretPos);

private:
	bool shoot;
};

#endif 