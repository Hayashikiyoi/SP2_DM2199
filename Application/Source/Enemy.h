#ifndef ENEMY_H
#define ENEMY_H

#include "MatrixStack.h"
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();
	Enemy(string Name, Vector3 Position);
	virtual ~Enemy();
	float RotateToPlayer(Vector3 player);

	bool isdead();
	void dmgToEnemy( float dmg);
	float showHP();

private:
	float health;
};

#endif