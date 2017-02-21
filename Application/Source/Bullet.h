#ifndef BULLET_H
#define BULLET_H
#include"GameObject.h"
#include"Enemy.h"

class Bullet : public GameObject
{	
public:
	Bullet();
	Bullet(string name, Vector3 curpos);
	virtual ~Bullet();
	void shootBullet(float rotate, Vector3 turretPos);
	void bulletUpdate(double dt);
	bool shoot ;
private:
	

};

#endif 