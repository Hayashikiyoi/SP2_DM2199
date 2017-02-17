#ifndef BULLET_H
#define BULLET_H
#include"Enemy.h"

class Bullet : public Enemy
{	
public:
	Bullet();
	~Bullet();
	void shootBullet(bool &check,float rotate, double &dt,Vector3& turretPos);

};

#endif 