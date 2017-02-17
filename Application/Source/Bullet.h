#ifndef BULLET_H
#define BULLET_H
#include"GameObject.h"

class Bullet : public GameObject
{	
public:
	Bullet();
	virtual ~Bullet();
	void shootBullet(bool &check,float rotate, double &dt,Vector3& turretPos);

};

#endif 